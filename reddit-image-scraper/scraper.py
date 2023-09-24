#!/usr/bin/env python3

import requests
import json
import time
import datetime
import os
import click

PS_REDDIT_BASE_URL = 'https://api.pushshift.io/reddit/search/submission/?title='
PS_REDDIT_SIZE_URL = '&size='
PS_REDDIT_AFTER_URL = '&after='
PS_REDDIT_BEFORE_URL = '&before='
PS_REDDIT_SUB_URL = '&subreddit='

MAX_WAIT_TIME = 3


class RedditPost():
    def __init__(self, title, url, author, sub_id, created, permalink, gallery_data={}):
        self.title = title
        self.url = url
        self.author = author
        self.sub_id = sub_id
        self.created = created
        self.permalink = permalink
        self.gallery_data = gallery_data


class RedditImageScraper():

    def __init__(self, out_folder):
        self.data = {}
        self.out_folder = out_folder
        self.num_images = 0

    def download_image(self, post):
        content = None
        downloader = ImageDownloader(post, self.out_folder)
        result = downloader.download()
        if result == 0:
            date = datetime.datetime.fromtimestamp(int(post.created))
            self.num_images += 1
            print("%d\t%s\t%s" % (self.num_images, date, post.url))

    def query_pushshift(self, sub, query, after, before):
        ps_data = self.get_pushshift_data(query, 1000, after, before, sub)
        post_count = 0

        while len(ps_data) > 0:
            for d in ps_data:
                post = self.get_post(d)
                self.download_image(post)
                post_count += 1

            after = ps_data[-1]['created_utc']
            ps_data = self.get_pushshift_data(query, 1000, after, before, sub)

    def get_pushshift_data(self, query, size, after, before, sub):
        url = (PS_REDDIT_BASE_URL + str(query) +
                PS_REDDIT_SIZE_URL + str(size) +
                PS_REDDIT_AFTER_URL + str(after) +
                PS_REDDIT_BEFORE_URL + str(before) +
                PS_REDDIT_SUB_URL + str(sub))
        r = requests.get(url)
        data = json.loads(r.text)
        return data['data']

    def get_post(self, json_data):  
        post = RedditPost(json_data['title'], 
                            json_data['url'],
                            json_data['author'],
                            json_data['id'],
                            json_data['created_utc'],
                            json_data['permalink'])

        if 'gallery_data' in json_data:
            post.gallery_data = json_data['gallery_data']

        return post


class ImageDownloader():

    def __init__(self, post, out_folder):
        self.post = post
        self.url = post.url
        self.out_folder = out_folder

    def download(self):
        result = -1
        if "/i.redd.it/" in self.url:
            result = self.direct_download()
        elif "/www.reddit.com/gallery/" in self.url:
            result = self.gallery_download()
        # TODO Add more
        return result

    def gallery_download(self):
        result = -1
        extensions = ['.jpg', '.jpeg', '.png']
        if not 'items' in self.post.gallery_data:
            return -1
        for item in self.post.gallery_data['items']:
            media_id = item['media_id']
            for e in extensions:
                url = 'https://i.redd.it/%s%s' % (media_id, e)
                img_data = self.get_image_data(url)
                if self.save_file(self.post.sub_id + "_" + media_id + url[-4:], img_data) == 0:
                    result = 0
        return 0 if result == 0 else -1

    def direct_download(self):
        img_data = self.get_image_data(self.url)
        fname = self.post.sub_id + self.url[-4:]
        return self.save_file(fname, img_data)

    def get_image_data(self, url):
        wait_time = 0
        while True:
            try:
                response = requests.get(url)
                if response.status_code == 200 and response.content:
                    return response.content
                elif response.status_code in (408, 429):
                    return None
                else:
                    return None
            except requests.exceptions.ConnectionError:
                time.sleep(1)
                wait_time += 1
                if wait_time > MAX_WAIT_TIME:
                    return None

    def save_file(self, name, data):
        if data:
            with open(os.path.join(self.out_folder, name), 'wb') as handler:
                handler.write(data)
            return 0
        return -1


@click.command()
@click.option("--sub", "sub", required=True, 
    help="Subreddit to scrape.")
@click.option("--after", "after", required=True, 
    type=click.DateTime(formats=["%Y/%m/%d", "%Y/%m/%d-%H:%M:%S"]),
    help="Start date to scrape from. Format: YYYY/mm/dd-HH:MM:SS")
@click.option("--before", "before", required=True, 
    type=click.DateTime(formats=["%Y/%m/%d", "%Y/%m/%d-%H:%M:%S"]),
    help="End date to scrape to. Format: YYYY/mm/dd-HH:MM:SS")
@click.option("--out-folder", "out_folder", default="out/[SUBNAME]",
    help="Folder to save images to.")
@click.option("--query", "query", default="", 
    help="Search query.")
def scrape(sub, after, before, out_folder, query):
    d1_unix = str(int(time.mktime(after.timetuple())))
    d2_unix = str(int(time.mktime(before.timetuple())))

    if out_folder == "out/[SUBNAME]":
        out_folder = out_folder.replace("[SUBNAME]", sub)

    if not os.path.isdir(out_folder):
        os.makedirs(out_folder)

    scraper = RedditImageScraper(out_folder)
    scraper.query_pushshift(sub, query, d1_unix, d2_unix)


if __name__ == "__main__":
    scrape()
# ai-essay-gen

Small script for generating a short video essay on a random fictional topic. The essay is generated using GPT-3 and a sequence of images are created from the text. Google's text-to-speech API is used as an AI narrator for the essay.

# Usage

Simply run:

```
python3 ai-essay-gen.py
```
And a folder will be created one above the ai-essay-gen folder containing the video essay (and supporting resources). 

### API Keys
You'll need to setup accounts for openai, stable diffusion and google cloud (and setup a project for text-to-speech). Once you've done that, make a folder one above the ai-essay-gen folder named "api_keys". This is where the api keys for the respective services go. They must be called: 
- openai_api_key
- stablediffusion_api_key
- googlecloud_api_key.json

### Prompts
essay_prompt.txt, image_prompt.txt, image_prompt_main.txt and image_style_prompt.txt can be edited to change the content of the video essay. 
- essay_prompt.txt contains the prompt that determines the essay text itself
- image_prompt_main.txt extracts five keys words from the entire essay that are prepended to all image prompts
- image_prompt.txt contains the prompt that generates the text from each paragraph used to prompt stable diffusion
- image_style_prompt.txt contains the text that is appended to the stable diffusion prompt to alter the image style

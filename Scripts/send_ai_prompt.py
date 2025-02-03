#import unreal
import requests
import json

url = 'http://localhost:11434/api/generate'
model = "gemma2:latest"
body_data = '''
{
  "model" : "gemma2:latest",
  "stream" : false,
  "prompt" : "Hello!"
}
'''
response = requests.post(url, data=body_data)



print(response.content["response"])
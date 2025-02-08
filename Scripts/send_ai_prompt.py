#import unreal
import requests
import json

def send_rest_call(prompt, model="gemma2:latest", url="http://localhost:11434/api/generate",stream=False) -> str:
  body_data = '''
  {
    "model" : "'''+model+'''",
    "stream" : false,
    "prompt" : "'''+prompt+'''"
  }
  '''

  try:
    response_data = requests.post(url, data=body_data)
  except:
    return "Unable to connect to "+url

  json_response_data = json.loads(response_data.content)

  if "error" in json_response_data:
    return json_response_data["error"]

  return json_response_data["response"]

def is_valid_json(myjson):
  try:
    json.loads(myjson)
  except ValueError as e:
    return False
  return True

def ask_to_ai(prompt) -> str:
  response = send_rest_call(prompt)

  #checks if is a working json
  brackets_index = 0
  first_open_bracket = response.find('{')
  last_bracket_index = -1
  for i, char in enumerate(response):
    if char == '{':
      brackets_index+=1
    elif char == '}':
      brackets_index-=1
      if brackets_index == 0:
        last_bracket_index = i + 1
        break

  if last_bracket_index == -1 :
    #there isn't a json inside this message
    return response,"No Json Requested"

  minesweeper_field_json = response[first_open_bracket:last_bracket_index].replace('\n','').replace('\'','\"')

  if is_valid_json(minesweeper_field_json):
    return response,minesweeper_field_json
  else:
    return response,"Invalid Json"

minesweeper_prompt = "Generate a new 3x3 Minesweeper grid with 2 mines. Make sure that the result you gave is a valid json and that all the Minesweeper\'s mine board generation rules are matching! The response must look like this : { 'grid': [ [1, 'X', 1], [2, 2, 1], ['X', 1, 0] ] }"

result = ask_to_ai("write me a a poem json")

print(result[0])
print(result[1])

#cacca = "{\"poem\": \"The moon, a silver coin in night's dark purse,\nShines on the world with gentle, silent verse.\nStars like diamonds, scattered far and wide,\nTwinkle secrets that the darkness hides.\n\nA hushed wind whispers through the sleeping trees,\nCarrying dreams on wings of gentle breeze.\nThe world is quiet, bathed in silver light,\nA moment stolen from the day's fierce fight.\"}"

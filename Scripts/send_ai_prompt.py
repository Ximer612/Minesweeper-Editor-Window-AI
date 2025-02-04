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

  response_data = requests.post(url, data=body_data)
  json_response_data = json.loads(response_data.content)
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
    #not asked for a minesweeper minigame
    return response

  minesweeper_field_json = response[first_open_bracket:last_bracket_index].replace('\n','').replace('\'','\"')

  if is_valid_json(minesweeper_field_json):
    return minesweeper_field_json
  else:
    return "Invalid json : " + response

minesweeper_prompt = "Generate a new 3x3 Minesweeper grid with 2 mines. Make sure that the result you gave is a valid json and that all the Minesweeper\'s mine board generation rules are matching! The response must look like this : { 'grid': [ [1, 'X', 1], [2, 2, 1], ['X', 1, 0] ] }"

print(ask_to_ai("write me a working json"))
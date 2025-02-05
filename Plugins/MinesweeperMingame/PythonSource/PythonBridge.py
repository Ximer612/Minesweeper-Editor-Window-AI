import unreal
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

def ask_to_ai(prompt) -> tuple[str, str]:
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


@unreal.uclass()
class PythonBridgeImplementation(unreal.PythonBridge):

    @unreal.ufunction(override=True)
    def function_implemented_in_python(self) -> str:
        return ask_to_ai("How are you feeling inside Unreal Engine 5?")[0]
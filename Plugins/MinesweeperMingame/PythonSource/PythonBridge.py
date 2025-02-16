import unreal
import requests
import json

def send_rest_call(prompt, model="gemma2:latest", url="http://localhost:11434/api/generate",stream=False) -> str:

  body_data = {
    "model" : model,
    "stream": stream,
    "prompt" : prompt    
  }

  payload = json.dumps(body_data)

  try:
    response_data = requests.post(url, payload)
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

#working with gemma2
def find_json_in_text(text:str):

  text = text.replace('\n','')

  json_header='```json'
  json_footer='```'

  first_open_bracket = text.find(json_header)

  if first_open_bracket == -1:
    return "none"

  first_open_bracket += + len(json_header)

  last_bracket_index = text.find(json_footer,first_open_bracket)

  json_result = text[first_open_bracket:last_bracket_index]

  return json_result

def ask_to_ai(prompt) -> tuple[str, str]:
  response = send_rest_call(prompt)

  #checks if is a working json
  json_result = find_json_in_text(response)

  if json_result == "none":
    return response,"none"

  if is_valid_json(json_result):
    return response,json_result
  else:
    return response,"invalid"

@unreal.uclass()
class PythonBridgeImplementation(unreal.PythonBridge):

    @unreal.ufunction(override=True)
    def ask_to_ai_python(self, prompt) -> unreal.PythonResult:
        print("I'm thinking about your prompt: \""+prompt+"\"...")
        #return unreal.PythonResult("TEST RESPONSE",'{ "grid": [  [1, "X", 1],  [2, 1, 1],  ["X", 1, 0] ]}')
        response = ask_to_ai(prompt)
        return unreal.PythonResult(response[0],response[1])
import time
import unreal
import requests
import json

#send rest call to the wanted ai model
def send_rest_call(prompt, model, url) -> tuple[str, bool]:

  body_data = {
    "model" : model,
    "stream": False,
    "prompt" : prompt    
  }

  payload = json.dumps(body_data)

  try:
    response_data = requests.post(url, payload)
  except:
    return "Unable to connect to "+url, True

  json_response_data = json.loads(response_data.content)

  if "error" in json_response_data:
    return json_response_data["error"], True

  return json_response_data["response"], False

#checks if a json is valid
def is_valid_json(myjson):
  try:
    json.loads(myjson)
  except ValueError as e:
    return False
  return True

#working with ai that send json code between ```
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

def ask_to_ai_and_parse_json(prompt,model,url) -> tuple[str, str, bool]:
  response, has_error = send_rest_call(prompt,model,url)

  if has_error:
    return response,"none",True

  #checks if is a working json
  json_result = find_json_in_text(response)

  if json_result == "none":
    return response,"none",False

  return response, json_result if is_valid_json(json_result) else "invalid", False #False because the response was successfully

def get_stream(prompt, model, url, python_bridge : unreal.PythonBridge):

  #checks if url is on
    try:
        requests.get(url)
    except Exception as e:
        unreal.log_error(e)
        return str(e),True
  
    s = requests.Session()

    body_data = {
        "model" : model,
        "stream": True,
        "prompt" : prompt    
    }

    payload = json.dumps(body_data)
    full_response = ""

    with s.post(url, data=payload, headers=None, stream=True) as resp:

      if resp.status_code != 200:
        full_response = "Error trying to connect with " + url
        unreal.log_error(full_response)
        return full_response, True

      for line in resp.iter_lines():
          if line:
              string_line = json.loads(line)["response"]
              full_response+=string_line
              python_bridge.update_text_block(string_line)
      unreal.log("Done!")

    return full_response, False


@unreal.uclass()
class PythonBridgeImplementation(unreal.PythonBridge):

    @unreal.ufunction(override=True)
    def ask_to_ai_python(self, prompt) -> unreal.PythonResult:

        unreal.log("I'm thinking about your prompt: \""+prompt+"\"...")

        if self.ai_stream:
          full_response, has_error = get_stream(prompt,self.ai_model,self.ai_url,self)

          return unreal.PythonResult(full_response,find_json_in_text(full_response),has_error)
        else:
          response, json, has_error = ask_to_ai_and_parse_json(prompt,self.ai_model,self.ai_url)
          return unreal.PythonResult(response,json,has_error)
from flask import Flask, request
import json

app = Flask(__name__)


@app.route('/')
def index():
    parsed = read_best_function()
    return "<html>" \
           "<body>" \
           "<center> <b> <h1>" \
           "Now best result is %s" \
           "<br>" \
           "With function %s" \
           "<br>" \
           "function number is %s" \
           "</h1></b></center>" \
           "</body>" \
           "</html>" % (parsed["children"], parsed["function"], parsed["function_number"])


@app.route('/get')
def getter():
    return str(read_best_function())


@app.route('/set', methods=["POST"])
def setter():
    children = request.args.get('children')
    function = request.args.get('function')
    function_number = request.args.get('function_number')
    print(children, function_number, function)
    if children is None or function is None or function_number is None:
        return "Error!"

    function_dict = {
        "children": children,
        "function": function,
        "function_number": function_number
    }
    return write_best_function(function_dict)


def read_best_function():
    f = open("../BEST_FUNCTION.json", "r")
    parsed = json.loads(f.read())
    f.close()
    return parsed


def write_best_function(function_dict):
    bf = read_best_function()
    if bf["children"] < function_dict["children"]:
        f = open("../BEST_FUNCTION.json", "w")
        f.write(json.dumps(function_dict))
        f.close()
        return "Updated"
    return "Old is better"


app.run()

import time
import requests
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-c", "--children", required=True,
                    help="")
parser.add_argument("-f", "--function", required=True,
                    help="")
parser.add_argument("-n", "--function_number", required=True,
                    help="")

args = parser.parse_args()
host = "192.168.100.9"
port = "5000"
url = "http://{}:{}/set".format(host, port)

n_retry = 5

cur_retry = 0
while cur_retry != n_retry:
    try:
        print("Success!")
        print(requests.post(url, params={
            "children": args.children,
            "function": args.function,
            "function_number": args.function_number
        }).text)

        break
    except Exception as e:
        print("Caught exception: {}".format(e))
        time.sleep(2)
        cur_retry += 1
else:
    exit(1)

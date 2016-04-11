from __init__ import api
from endpoints import *
import sys

if __name__ == "__main__":

    # check if the port number that is used to run this script has been
    # provided or not
    if len(sys.argv) == 2:
        port = sys.argv[1]
    else:
        sys.stderr.write("Usage: python " + sys.argv[0] + " <port_number>\n")
        sys.exit(1)

    api.run(debug = True, port = int(sys.argv[1]), threaded = True)

import json
from flask import Response
from functools import wraps

def api_jsonify(fn):
    """Use this function as a decorator on a function that returns a
    Python dictionary or something that is json.dumps()-able.  This then takes
    the response from a call to json.dumps() and returns it with the
    appropriate mimetype set"""
    @wraps(fn)
    def function_wrapper(*args, **kwargs):

        # return the response object constructed with the json.dumps from the
        # call to another function that returns an array or a simple
        # dictionary
        return Response(json.dumps(fn(*args, **kwargs)), mimetype = \
                "application/json")

    return function_wrapper


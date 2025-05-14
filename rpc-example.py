import msgpack

# Define the server-side function
def add(a, b):
    return a + b

# Define the server-side function to handle RPC calls
def server(packed_data):
    # Unpack the received data using msgpack
    unpacked_data = msgpack.unpackb(packed_data)
    
    # Extract function name and arguments
    function_name = unpacked_data["function_name"]
    args = unpacked_data["args"]
    
    # Call the appropriate function based on the function name
    if function_name == "add":
        result = add(*args)
    
    # Pack the result using msgpack and return it
    packed_result = msgpack.packb(result)
    
    return packed_result

# Define the client-side function to make RPC call
def rpc_call(function_name, args):
    # Pack the function name and arguments using msgpack
    packed_data = msgpack.packb({"function_name": function_name, "args": args})
    
    # Simulate sending the packed data to the server and receiving a response
    response = server(packed_data)
    
    # Unpack the response using msgpack
    unpacked_response = msgpack.unpackb(response)
    
    return unpacked_response

# Example usage: Client making an RPC call to add two numbers
result = rpc_call("add", [5, 3])
print(f"Result of RPC call: {result}")

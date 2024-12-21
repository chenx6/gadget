from pyinfra.operations import server

server.shell(
    name="Hello pyinfra",
    commands=['echo "hello from $HOST $USER"'],
)

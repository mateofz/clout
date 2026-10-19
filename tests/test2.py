
#!/usr/bin/env python3

import random
import sys

messages = [
    "hello world",
    "warning occurred",
    "processing request",
    "something went wrong",
    "connection established",
    "debug message",
    "operation completed",
    "file not found",
]

for i in range(1, 5):
    message = random.choice(messages)
    stream = random.choice([
        ("stdout", sys.stdout),
        ("stderr", sys.stderr),
    ])

    name, fd = stream
    print(f"[{i}] [{name}] {message}", file=fd, flush=True)

    user_input = input()
    print(f"[{i}] [stdout] input: {user_input}", file=sys.stdout, flush=True)


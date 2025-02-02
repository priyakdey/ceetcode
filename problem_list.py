import os
from typing import List, Tuple


root_dir = "./"

problems: List[Tuple[int, str, str, str]] = []

for filename in os.listdir(root_dir):
    if filename.endswith(".c"):
        with open(filename, "r") as fp:
            desc = fp.readline().strip()[3:]
            link = fp.readline().strip()[3:]
            _id = int(desc.split(" ")[0][:-1])
            
            problems.append((_id, desc, link, filename))
            

problems.sort(key=lambda x: x[0])

print("| Id | Problem | Filename |")
print("| -- | ------- | -------- |")

for _id, desc, link, filename in problems:
    print(f"| {_id} | [{desc}]({link}) | [{filename}]({"./" + filename})")

    
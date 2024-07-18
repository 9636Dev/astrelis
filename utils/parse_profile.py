import json
import sys
from dataclasses import dataclass

class Profile:
    name: str
    start: int
    time: int

    def __init__(self, name: str, start: int, time: int):
        self.name = name
        self.start = start
        self.time = time

class ProfileTree:
    profile: Profile
    children: list

    def __init__(self, profile: Profile):
        self.profile = profile
        self.children = []

    def add_child(self, child):
        self.children.append(child)

    def print_tree(self, depth: int, parentDuration: int, totalDuration: int):
        print('  ' * depth + f'{self.profile.name} {(self.profile.time / 1000.0):.4f}ms {(self.profile.time / parentDuration * 100):.2f}% ({self.profile.time / totalDuration * 100:.2f}%)')
        for child in self.children:
            child.print_tree(depth + 1, self.profile.time, totalDuration)

def construct_tree(profiles: list[Profile]):
    root = ProfileTree(profiles[0])
    traversalStack = [(root, root.profile.start + root.profile.time)]

    for i in range(1, len(profiles)):
        profile = profiles[i]

        while traversalStack:
            parent, parentEnd = traversalStack[-1]

            if profile.start < parentEnd:
                child = ProfileTree(profile)
                parent.add_child(child)
                traversalStack.append((child, profile.start + profile.time))
                break
            else:
                traversalStack.pop()

    return root


def calculate_no_constraints(data):
    totalDuration: int = data['totalDuration']

    profiles = []
    for profile in data['profiles']:
        profiles.append(Profile(profile['name'], profile['timestamp'], profile['duration']))

    # Sort by start time, and if same by longest duration
    profiles.sort(key=lambda x: (x.start, -x.time))

    # We add a dummy profile at the start that is the same start time as the first function, but has a duration of totalDuration
    profiles.insert(0, Profile('root', profiles[0].start, totalDuration))

    profileTree: ProfileTree = construct_tree(profiles)
    profileTree.print_tree(0, profileTree.profile.time, totalDuration)

def main():
    # The script argument should be the profile
    if len(sys.argv) < 2:
        print('Usage: python parse_profile.py <profile>')
        sys.exit(1)

    profile = sys.argv[1]

    scope: None | str = None
    if len(sys.argv) == 3:
        scope = sys.argv[2]
    elif len(sys.argv) > 3:
        print('Usage: python parse_profile.py <profile> <scope>')
        sys.exit(1)


    with open(profile, 'r') as f:
        data = json.load(f)

    if scope is None:
        calculate_no_constraints(data)

if __name__ == '__main__':
    main()

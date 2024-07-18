import json
import sys
from dataclasses import dataclass

class Profile:
    name: str
    times: list[int]

    def __init__(self, name: str):
        self.name = name
        self.times = []

def main():
    # The script argument should be the profile
    if len(sys.argv) != 2:
        print('Usage: python parse_profile.py <profile>')
        sys.exit(1)

    profile = sys.argv[1]
    with open(profile, 'r') as f:
        data = json.load(f)

    totalTime = data['totalDuration']
    profiles = {}
    for profile in data['profiles']:
        # Find the profile in 'profiles', otherwise create
        if not profiles.get(profile['name']):
            profiles[profile['name']] = Profile(profile['name'])

        prof = profiles[profile['name']]
        prof.times.append(profile['duration'])

    print('Total time: ' + str(totalTime / 1000.0) + "ms")

    # For each profile, print the average time, times ran, and the percentage of total time
    for prof in profiles.values():
        avgTime = sum(prof.times) / len(prof.times) / 1000
        timesRan = len(prof.times)
        percentage = sum(prof.times) / totalTime * 100
        print('Profile: ' + prof.name)
        print('Average time: ' + str(avgTime) + "ms")
        print('Times ran: ' + str(timesRan))
        print('Percentage of total time: ' + str(percentage) + '%')
        print()

if __name__ == '__main__':
    main()

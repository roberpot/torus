#!/usr/bin/python3

import argparse
import os.path


class Defname(object):
    def __init__(self, line):
        self._line = line
        self._groups = []

    def create_group(self):
        if (len(self._groups) > 0 and self._groups[-1].line_count() > 0) or \
           len(self._groups) == 0:
            self._groups.append(DefnameGroup())

    def add_line(self, line):
        if len(self._groups) == 0:
            self.create_group()
        self._groups[-1].add_line(line)

    def __str__(self):
        ret = "%s\n" % self._line
        ret += "\n".join(["%s" % x for x in self._groups])
        return ret


class DefnameGroup(object):
    def __init__(self):
        self._lines = []

    def add_line(self, line):
        self._lines.append(DefnameGroupLine(line))

    def line_count(self):
        return len(self._lines)

    def __str__(self):
        if len(self._lines) == 0:
            return '\n'
        size = max([x.attr_len() for x in self._lines]) + 1
        for x in self._lines:
            x.adjust(size)
        return "%s\n" % '\n'.join(["%s" % x for x in self._lines])


class DefnameGroupLine(object):
    def __init__(self, line):
        self._full_line = line
        self._attr_len = 0
        self._comment = ""
        self._attr = ""
        self._value = ""
        if line.startswith("//"):
            return
        self._comment = ""
        if "//" in line:
            line, self._comment = line.split("//")
            self._comment = "  //%s" % self._comment
        line = [x for x in line.split(' ') if x != ""]
        self._attr = line[0]
        self._attr_len = len(self._attr)
        self._value = ' '.join(line[1:])

    def attr_len(self):
        return self._attr_len

    def adjust(self, size):
        if self._attr_len > 0:
            self._attr = self._attr.strip() + (' ' * (size - self._attr_len))

    def __str__(self):
        if self._attr_len == 0:
            return self._full_line
        ret = "%s= %s;" % (self._attr, self._value)
        if len(self._comment):
            ret += self._comment
        return ret


def main():
    parser = argparse.ArgumentParser(
        description='Updates a sphere defname to torus defname.')
    parser.add_argument(
        '--file', '-f',
        dest='file',
        required=True,
        type=str,
        help='file to update')
    args = parser.parse_args()
    if not os.path.isfile(args.file):
        print("File \"%s\" does not exists." % args.file)
        return 0
    file = open(args.file, "r")
    defnames = []
    for line in file:
        line = line.replace('\n', '')
        line = line.replace('\t', ' ')
        line = line.strip()
        if "[" in line and "DEFNAME" in line:
            defnames.append(Defname(line))
        elif "[" in line and "TYPEDEF" in line:
            defnames.append(Defname(line))
        elif len(line) == 0 and len(defnames) > 0:
            defnames[-1].create_group()
        elif len(line) > 0 and len(defnames) > 0:
            defnames[-1].add_line(line)

    for defname in defnames:
        print(defname)


if __name__ == "__main__":
    main()

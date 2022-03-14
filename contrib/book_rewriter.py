#!/usr/bin/python3

import argparse
import os.path


def sanitize_line(line):
    return line.replace('\"', "'")


class Page(object):
    def __init__(self, line):
        self._number = line.split(' ')[2][:-1]
        self._lines = []

    def add_line(self, line):
        self._lines.append("\"%s\"" % sanitize_line(line))

    def __str__(self):
        ret = "page %s = {\n" % self._number
        ret += "\n".join(self._lines)
        ret += "\n}"
        return ret


class Book(object):
    class InvalidPage(Exception):
        pass

    def __init__(self, line):
        self._line = line
        self._comments = []
        self._page_count = 0
        self._title = ""
        self._author = ""
        self._pages = []

    def add_comment(self, line):
        self._comments.append("%s\n" % line)

    def add_page(self, line):
        self._pages.append(Page(line))

    def add_info(self, line):
        key, value = line.split('=')
        if key == "PAGES":
            self._page_count = value
        elif key == "TITLE":
            self._title = value
        elif key == "AUTHOR":
            self._author = value

    def add_line(self, line):
        if len(self._pages) == 0:
            raise Book.InvalidPage()
        self._pages[-1].add_line(line)

    def __str__(self):
        ret = "\n%s\n" % self._line
        ret += "pages = %s;\n" % self._page_count
        ret += "title = \"%s\";\n" % self._title
        ret += "author = \"%s\";\n" % self._author
        ret += "\n".join(self._comments)
        ret += "\n".join(["%s" % x for x in self._pages])
        return ret


def main():
    parser = argparse.ArgumentParser(
        description='Updates a sphere books to torus defname.')
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
    books = []
    success = True
    for lineno, line in enumerate(file):
        try:
            line = line.replace('\n', '')
            line = line.replace('\t', ' ')
            line = line.strip()
            if "[" in line and "BOOK" in line and len(line.split(' ')) == 2:
                books.append(Book(line))
            elif "[" in line and "BOOK" in line and len(line.split(' ')) > 2 and len(books) > 0:
                books[-1].add_page(line)
            elif line.startswith('//') and len(books) > 0:
                books[-1].add_comment(line)
            elif len(line.split('=')) == 2 and len(books) > 0:
                books[-1].add_info(line)
            elif len(line) > 0:
                books[-1].add_line(line)
        except Book.InvalidPage:
            print("Invalid page at line %s: %s" % (lineno, line))
            success = False

    if success:
        for book in books:
            print(book)


if __name__ == "__main__":
    main()

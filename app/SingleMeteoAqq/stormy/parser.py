__author__ = 'Quepas'

from html.parser import HTMLParser
from xml.parsers.expat import XMLParserType
from xml.parsers.expat import ParserCreate
import xml.etree.ElementTree as ETree
import re

class MeteoADataParser(HTMLParser):
    allData = []

    validData = []
    validDataIndex = []

    validLabels = []
    validLabelsIndex = []

    def handle_data(self, data):
        self.allData.append((self.getpos()[1], data))

    def handle_starttag(self, tag, attrs):
        globalOffset = self.getpos()[1]
        fullTag = str(self.get_starttag_text())
        dataOffset = globalOffset + len(fullTag)

        if tag == "font" and ('color', 'Brown') in attrs:
            self.validLabelsIndex.append(dataOffset)
        if tag == "font" and ('color', '#3366FF') in attrs:
            self.validDataIndex.append(dataOffset)

    def get_meteo_data(self):
        lables = self.get_meteo_labels()
        values = self.get_meteo_values()
        result = []

        for i in range(self.validLabelsIndex.__len__()):
            result.append((lables[i], values[i]))

        return result

    def get_meteo_labels(self):
        result = []
        for data in self.allData:
            if data[0] in self.validLabelsIndex or data[0]-7 in self.validLabelsIndex:
                result.append(data[1])
        return result

    def get_meteo_values(self):
        result = []
        for data in self.allData:
            if data[0] in self.validDataIndex or data[0]-7 in self.validDataIndex:
                result.append(data[1])
        return result

class MeteoBDataParser(HTMLParser):
    data = {}

    def parse(self, fileName, encoding="UTF-8"):
        text = open(fileName, encoding=encoding).read()
        content = self.extractTable(text)
        root = ETree.fromstring(content)
        if root.tag == "tbody":
            self.parseData(root)

    def parseText(self, text, encoding="UTF-8"):
        content = self.extractTable(text)
        print(self.addTBodyTags(content))
        root = ETree.fromstring(self.addTBodyTags(content))
        if root.tag == "tbody":
            self.parseData(root)

    def parseData(self, root):
        for child in root:
            if child.tag == "tr":
                row = self.parseSingleRow(child)
                if row != None:
                    self.data.update(row)

    def parseSingleRow(self, tr):
        if len(tr) == 2:
            return {tr[0][0].text : tr[1][0].text}

    def extractBody(self, text):
        lowerText = text.lower()
        bodyStart = lowerText.find("<body>")
        if bodyStart > -1:
            bodyEnd = lowerText.rfind("</body>")
            return text[bodyStart+6 : bodyEnd].strip()

    def extractTable(self, text):
        lowerText = text.lower()
        tableTagStart = lowerText.find("<table>")
        if tableTagStart > -1:
            tableTagEnd = lowerText.rfind("</table>")
            return text[tableTagStart+7 : tableTagEnd].strip()

    def removeDiv(self, text):
        lowerText = text.lower()
        divEndTag = lowerText.find("</div>")
        if divEndTag > -1:
            divEndTag += 6
            return text[divEndTag:]

    def addTBodyTags(self, text):
        return "<tbody>" + text + "</tbody>"

class MeteoStationParser():
    parser = ParserCreate()

    def __init__(self):
        self.parser.StartElementHandler = self.handle_startelem
        self.parser.EndElementHandler = self.handle_endelem
        self.parser.CharacterDataHandler = self.handle_data

    def handle_startelem(self, name, attrs):
        if name == "station":
            print("Start: ")
            print(name)
            print(attrs)
    def handle_endelem(self, name): pass
        #print("End: ")
        #print(name)
    def handle_data(self, data): pass
        #print("Data: " + data)

    def Parse(self, data):
        self.parser.Parse(data)


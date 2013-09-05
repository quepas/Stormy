__author__ = 'Quepas'

from html.parser import HTMLParser
from xml.parsers.expat import XMLParserType
from xml.parsers.expat import ParserCreate
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
    pass

class MeteoStationParser():
    parser = ParserCreate()

    def __init__(self):
        self.parser.StartElementHandler = self.handle_startelem
        self.parser.EndElementHandler = self.handle_endelem
        self.parser.CharacterDataHandler = self.handle_data

    def handle_startelem(self, name, attrs):
        print("Start: ")
        print(name)
        print(attrs)
    def handle_endelem(self, name):
        print("End: ")
        print(name)
    def handle_data(self, data):
        print("Data: " + data)
    def Parse(self, data):
        self.parser.Parse(data)


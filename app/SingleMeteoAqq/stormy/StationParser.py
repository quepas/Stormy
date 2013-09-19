__author__ = 'Quepas'

import xml.etree.ElementTree as ETree

class StationParser():
    tree = None
    rawStations = []

    def parse(self, fileName):
        self.tree = ETree.parse(fileName)
        self.parseData()

    def parseData(self):
        root = self.tree.getroot()
        if root.tag == "stations":
            for child in root:
                self.rawStations.append(self.parseStation(child))

    def parseStation(self, station):
        result = {}
        if station.tag == "station":
            result["name"] = station[0].text
            result["coordinate"] = station[1].text
            result["url"] = station[2].text
        return result

    def getStationData(self):
        return self.rawStations

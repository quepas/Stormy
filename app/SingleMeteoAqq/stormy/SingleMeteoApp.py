__author__ = 'Quepas'

# reading data from http://zdpk.krakow.pl/pub/meteo/zdpk01/Current_Vantage.htm
# reading data from http://stacjameteo.pl/dane/index.php?stacja=XX

from stormy.parser import MeteoBDataParser
from stormy.StationParser import StationParser

stationParser = StationParser()
stationParser.parse("../../../data/meteorological_stations.xml")

meteoParse = MeteoBDataParser()

for station in stationParser.getStationData():
    print(station["url"])
    meteoParse.parseFromHtml(station["url"])
    print(meteoParse.data)







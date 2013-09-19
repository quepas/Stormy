__author__ = 'Quepas'

# reading data from http://zdpk.krakow.pl/pub/meteo/zdpk01/Current_Vantage.htm

from urllib import request
from stormy.parser import MeteoADataParser
from stormy.parser import MeteoBDataParser
from stormy.StationParser import StationParser
from stormy.parser import MeteoStationParser

parser = StationParser()
parser.parse("../../../data/meteorological_stations.xml")

meteoParse = MeteoBDataParser()
meteoParse.parse("../../../data/meteo_station/21. Wisła Malinka/Dane meteo stacji 21.htm")

for name in parser.getStationData():
    print(name["url"])
    sock = request.urlopen(name["url"]);
    htmlSource = str(sock.read(), encoding="UTF-8")
    print(htmlSource)
    htmlSource = htmlSource\
        .replace(" class=tr-0", "")\
        .replace(" class=tr-1", "")\
        .replace("&deg;", "°")\
        .replace("&sup2;", "²")
    meteoParse.parseText(htmlSource)
    print(meteoParse.data)
    sock.close()



__author__ = 'Quepas'

# reading data from http://zdpk.krakow.pl/pub/meteo/zdpk01/Current_Vantage.htm
# reading data from http://stacjameteo.pl/dane/index.php?stacja=XX

from urllib import request
from stormy.parser import MeteoBDataParser
from stormy.StationParser import StationParser

parser = StationParser()
parser.parse("../../../data/meteorological_stations.xml")

meteoParse = MeteoBDataParser()
meteoParse.parse("../../../data/meteo_station/21. Wisła Malinka/Dane meteo stacji 21.htm")

for name in parser.getStationData():
    print(name["url"])
    sock = request.urlopen(name["url"]);
    htmlSource = str(sock.read(), encoding="UTF-8")
    htmlSource = htmlSource\
        .replace(" class=tr-0", "")\
        .replace(" class=tr-1", "")\
        .replace("&deg;", "°")\
        .replace("&sup2;", "²")\
        .replace("<tr/>", "</tr>");
    meteoParse.parseText(htmlSource)
    print(meteoParse.data)
    sock.close()



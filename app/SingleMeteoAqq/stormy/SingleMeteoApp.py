__author__ = 'Quepas'

# reading data from http://zdpk.krakow.pl/pub/meteo/zdpk01/Current_Vantage.htm

from urllib import request
from stormy.parser import MeteoADataParser
from stormy.parser import MeteoStationParser

sock = request.urlopen("http://zdpk.krakow.pl/pub/meteo/zdpk01/Current_Vantage.htm");
htmlSource = str(sock.read())
sock.close()

parser = MeteoADataParser()
parser.feed(htmlSource)

print(parser.validLabelsIndex)
print(parser.validLabels)

print(parser.get_meteo_data())

parserXML = MeteoStationParser()
parserXML.Parse(
    str(open("../../../data/meteorological_stations.xml").read()))

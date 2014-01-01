__author__ = 'Quepas'

from urllib import request
from xml.dom import minidom

def run(text, encoding="UTF-8"):
    parser = AGH_MeteoParser()
    parser.parseFromURL(text, encoding)
    return parser.data

class AGH_MeteoParser():
    xml_dom = None
    data = []

    def parseFromURL(self, url, encoding="UTF-8"):
        self.data=[]
        sock = request.urlopen(url)
        content = str(sock.read(), encoding)
        self.parseXML(content)
        sock.close()

    def parseXML(self, content):
        self.xml_dom = minidom.parseString(content)
        self.getDataAs("ta", "Temperatura powietrza")
        self.getDataAs("ua", "Wilgotność powietrza")
        self.getDataAs("odew", "Punkt rosy")
        self.getDataAs("owindchill", "Temperatura odczuwalna")
        #self.getDataAs("oheatindex", "")
        self.getDataAs("sx", "Maksymalna prędkość wiatru")
        self.getDataAs("sm", "Prędkość średnia")
        self.getDataAs("dm", "Kąt kierunku wiatru")
        self.getDataAs("pa", "Ciśnienie atmosferyczne")
        self.getDataAs("barosealevel", "Ciśnienie barometryczne")
        self.getDataAs("rc", "Opad")
        self.getDataAs("hc", "Grad")
        self.getDataAs("ri", "Intensywność opadu")
        self.getDataAs("hi", "Intensywność gradu")
        self.getDataAs("tendency", "Tendencja")
        self.getDateTime()

    def getDataAs(self, tag_name, equivalent):
        self.data.append(
            {equivalent : self.xml_dom.getElementsByTagName(tag_name)[0].childNodes[0].nodeValue})

    def getDateTime(self):
        datetime = self.xml_dom.getElementsByTagName("dane_aktualne")[0].attributes["data"].value
        date = datetime.split(" ")[0]
        time = datetime.split(" ")[1]
        self.data.append({"Data" : date})
        self.data.append({"Czas" : time})
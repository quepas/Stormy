__author__ = 'quepas'
# AGH data reader
# python 2.7.6

from xml.dom import minidom

def process(text):
    parser = AGH_MeteoParser()
    parser.data = {}
    parser.parseXML(text)
    print(parser.data)
    return parser.data

class AGH_MeteoParser():
    xml_dom = None
    data = {}

    def parseXML(self, content):
        self.xml_dom = minidom.parseString(content)
        self.getDataAs("ta", "Temperatura powietrza")
        self.getDataAs("ua", "Wilgotnosc powietrza")
        self.getDataAs("odew", "Punkt rosy")
        self.getDataAs("owindchill", "Temperatura odczuwalna")
        #self.getDataAs("oheatindex", "")
        self.getDataAs("sx", "Maksymalna predkosc wiatru")
        self.getDataAs("sm", "Predkosc srednia")
        self.getDataAs("dm", "Kat kierunku wiatru")
        self.getDataAs("pa", "Cisnienie atmosferyczne")
        self.getDataAs("barosealevel", "Cisnienie barometryczne")
        self.getDataAs("rc", "Opad")
        self.getDataAs("hc", "Grad")
        self.getDataAs("ri", "Intensywnosc opadu")
        self.getDataAs("hi", "Intensywnosc gradu")
        self.getDataAs("tendency", "Tendencja")
        self.getDateTime()

    def getDataAs(self, tag_name, equivalent):
        self.data.update({
            equivalent.encode('unicode-escape') :
            self.xml_dom.getElementsByTagName(tag_name)[0].childNodes[0].nodeValue.encode('unicode-escape')
        })

    def getDateTime(self):
        datetime = self.xml_dom.getElementsByTagName("dane_aktualne")[0].attributes["data"].value
        date = datetime.split(" ")[0]
        time = datetime.split(" ")[1]
        self.data.update({"Data" : date.encode('unicode-escape')})
        self.data.update({"Czas" : time.encode('unicode-escape')})
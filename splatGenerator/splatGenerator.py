# generator broni , przeciwników do splattera ( bo komu by się chciało to pisać w C++)
import random,string, argparse, re
from math import ceil

# stałe których użyjemy do opisu danych
really_shitty   =  0.05
almost_none     =  0.1
very_low        =  0.25
low             =  0.5
average         =  0.8
normal          =  1.
rather_good     =  1.3
good            =  1.7
high            =  2.
very_high       =  4.
enormous        =  8.
pretty_awesome  =  12.


def printToFile(params, file):
    file.write("\n"+"*"*40 + "\n")
    for key in params:
        if  isinstance(params[key],int):
            #           przyznaję bez bicia że jest to zrobione bez sensu 
            #           ale to tylko tymczasowy skrypyt kek
            file.write(translateWpnType(str(params[key])) +"        *" + str(key)  +  "\n")
    file.write("*"*40 + "\n")
   
####################################################################################################################################################################
# Bronie
dictionary = {}
weaponTypeDict = {"Pistol":             {"cost":average, "minDmg":average,"maxDmg":average,"firerate":low,"magSize": very_low,"recoil":normal,"minSpeed":average,
                                        "maxSpeed":average,"bulletSize":average,"projItShoot":normal, "projItTakes":normal, "reloadingTime" : very_low},
                  
                  "Shotgun":            {"cost":high, "minDmg":high,"maxDmg":high,"firerate":very_low,"magSize": low,"recoil":high,"minSpeed":good,
                                        "maxSpeed":good,"bulletSize":low,"projItShoot":very_high, "projItTakes":good,  "reloadingTime" : good},

                  "Sniper":             {"cost":very_high, "minDmg":very_high,"maxDmg":enormous,"firerate":almost_none,"magSize": very_low,"recoil":almost_none,"minSpeed":enormous,
                                        "maxSpeed":enormous,"bulletSize":good,"projItShoot":very_low, "projItTakes":very_low,  "reloadingTime" : high},

                  "SMG":                {"cost":average, "minDmg":low,"maxDmg":normal,"firerate":very_high,"magSize": high,"recoil":high,"minSpeed":good,
                                        "maxSpeed":good,"bulletSize":low,"projItShoot":good, "projItTakes":good,  "reloadingTime" : low},

                  "Assault" :           {"cost":average, "minDmg":average,"maxDmg":high,"firerate":good,"magSize": good,"recoil":average,"minSpeed":good,
                                        "maxSpeed":high,"bulletSize":good,"projItShoot":good, "projItTakes":normal,  "reloadingTime" : normal},

                  "MachineGun" :        {"cost":very_high, "minDmg":high,"maxDmg":high,"firerate":high,"magSize": very_high,"recoil":enormous,"minSpeed":good,
                                        "maxSpeed":good,"bulletSize":average,"projItShoot":normal, "projItTakes":normal,  "reloadingTime" : high},

                  "Thrower"  :          {"cost":good, "minDmg":low,"maxDmg":normal,"firerate":enormous,"magSize": enormous,"recoil":very_high,"minSpeed":good,
                                        "maxSpeed":good,"bulletSize":low,"projItShoot":high, "projItTakes":high,  "reloadingTime" : high},

                  "Musket" :            {"cost":very_high, "minDmg":enormous,"maxDmg":pretty_awesome,"firerate":really_shitty,"magSize": really_shitty,"recoil":almost_none,"minSpeed":very_high,
                                        "maxSpeed":enormous,"bulletSize":good,"projItShoot":low, "projItTakes":enormous,  "reloadingTime" : high} ,
                 
                  "Caster"  :           {"cost":low, "minDmg":low,"maxDmg":average,"firerate":very_high,"magSize": average,"recoil":normal,"minSpeed":normal,
                                        "maxSpeed":normal,"bulletSize":normal,"projItShoot":good, "projItTakes":low,  "reloadingTime" : very_low},

                  "rocketLauncher" :    {"cost":very_high, "minDmg":pretty_awesome,"maxDmg":pretty_awesome,"firerate":really_shitty,"magSize": really_shitty,"recoil":enormous,"minSpeed":low,
                                        "maxSpeed":average,"bulletSize":high,"projItShoot":really_shitty, "projItTakes":pretty_awesome,  "reloadingTime" : high}
                  }
weaponBltsTypeDict =  {"lead":              {"cost":low, "minDmg":normal,"maxDmg":normal,"firerate":average,"magSize": good,"recoil":good,"minSpeed":average,
                                  "maxSpeed":average,"bulletSize":normal,"projItShoot":normal, "projItTakes":normal,  "reloadingTime" : average},

                   #                                                                                                    idziemy w miarę realistycznie lol 
                   #                                                                                                          ||
                   #                                                                                                          \/
                   "energetic" : {"cost":normal, "minDmg":low,"maxDmg":low,"firerate":high,"magSize": high,"recoil":really_shitty,"minSpeed":high,
                                  "maxSpeed":high,"bulletSize": low,"projItShoot":normal, "projItTakes":low,  "reloadingTime" : low},
                   
                   "plasma" :    {"cost":high, "minDmg":good,"maxDmg":high,"firerate":low,"magSize": low,"recoil":good,"minSpeed":low,
                                  "maxSpeed":low,"bulletSize":high,"projItShoot":low, "projItTakes":good,  "reloadingTime" : good}
                   }

# łańcuch z którego będziemy losować sobie iloma pociskami będzie broń strzelać
howManyProjItShootRndTbl = "1" * 60 + "2" * 20  + "3" * 10 + "4" * 7 + "5" * 3  

# łańcuch z ktorego będziemy sobie losować iloma będzie kosztować pojedyńczy strzał
howManyProjItTakesRndTbl = " 0.5 " * 30 + " 1 " * 55 + " 2 " * 15

# Szansa na efekt terenowy
TerrainEffectsRndTbl = "X" * 5 + "F" * 5 + "E" * 5 + "T" * 5 + "0" * 80

# szansa na jakieś szity typu celownik, stabilizator, powiększony magazynek
commonEffectsRndTbl = "1" * 20 + "0" * 80

# szansa na wylosowanie super efektów takich jak multipler, inteligentne pociski czy odbijające się pociski
awesomeEffectsRndTbl = "I" * 5 + "M" * 10 + "S"*10 + "F" * 15 + "0" * 60

# szansa na ręcznie wprowadzone (or should i said komputerowo wygenerowane) zasięg obszarówki
terrainRadiusRndTbl  = "4"*5 + "3"*5 + "2" * 10 + "1" * 20+ "0" * 60   

# szansa na wylosowanie efektów dla broni ołowiowej
leadWpnTypeRndTbl    = "G" * 10 + "F" * 10 + "0" * 80

# szansa na wylosowanie efektów dla broni elektrycznej
energeticWpnTypeRndTbl = "C" * 10 + "A" * 15 + "0" * 75

# szansa na wylosowanie efektów dla broni plazmowej
plasmaWpnTypeRndTbl    = "M" * 15 + "P" * 15 + "0" * 70

# szansa na "upakowanie" kulek 
packedBulletsRndTbl    = "1" * 45 + "0" * 55 
###################################################################
# Funkcje do Broni
def translateWpnType(type):
    
    if re.match(".*lead.*",type):
        return "0"
    elif re.match(".*energetic.*",type):
        return "1"
    elif re.match(".*plasma.*",type):
        return "2"
    else :
        return str(type)
def correctParams(params):
    # bardzo nie chcemy aby np. w rozmiarze magazynka pojawiło się zero 
    # (tzn. ustalamy najmniejsze i największe wartości oraz zaokrąglamy jeśli trzeba)
    params["cost"]               = ceil(params["cost"])
    params["minDmg"]             =   max (ceil(params["minDmg"]),1) 
    params["maxDmg"]             =   max (ceil(params["maxDmg"]),1)
    params["firerate"]           =   max (ceil(params["firerate"]),5)
    params["howManyProjItShoot"] =   max (ceil(params["howManyProjItShoot"]),1)
    params["howManyProjItTakes"] =   max (ceil(params["howManyProjItTakes"]),1)
    params["size"]               =   max (ceil(params["size"]),1)
    params["size"]               =   min(params["size"],20)
    params["magSize"]            =   max (ceil(params["magSize"]),params["howManyProjItTakes"])
    params["minSpeed"]           =   max (ceil(params["minSpeed"]),1)
    params["maxSpeed"]           =   max (ceil(params["maxSpeed"]),1)
    params["reloadingTime"]      =   ceil(params["reloadingTime"])
    params["recoil"]             =   min(45,  ceil(params["recoil"])) 
    params["minSpeed"]           =   min(300 ,  ceil(params["minSpeed"]))
    params["maxSpeed"]           =   min(300 ,  ceil(params["maxSpeed"]))
def generatePrimaryParams(params):
    # generujemy sobie podstawowe prametry takie jak szybkostrzelność, damage itp.
    params["name"]      = 0
    params["cost"]      = random.randint(100,300)
    params["minDmg"]    = random.randint(20,40)
    params["maxDmg"]    = random.randint(0,40) + params["minDmg"]
    params["firerate"]  = random.randint(600,800)
    params["magSize"]   = random.randint(30,100)
    params["size"]      = random.randint(5,20)
    params["howManyProjItShoot"] = int(random.choice(howManyProjItShootRndTbl))
    params["howManyProjItTakes"] = params["howManyProjItShoot"]  * float(random.choice(howManyProjItTakesRndTbl.split()))
    params["reloadingTime"]      = random.randint(1,3)
    params["recoil"]             = random.randint(5,10)
    params["minSpeed"]           = random.randint(200, 300)
    params["maxSpeed"]           = params["minSpeed"] + random.randint(100,200)    
def applyConstantsValues(params, bltType, wpnType):
    params["cost"]               *= weaponBltsTypeDict[bltType]["cost"] * weaponTypeDict[wpnType]["cost"]
    params["minDmg"]             *= weaponBltsTypeDict[bltType]["minDmg"] * weaponTypeDict[wpnType]["minDmg"]
    params["maxDmg"]             *= weaponBltsTypeDict[bltType]["maxDmg"] * weaponTypeDict[wpnType]["maxDmg"]
    params["firerate"]           *= weaponBltsTypeDict[bltType]["firerate"] * weaponTypeDict[wpnType]["firerate"]
    params["magSize"]            *= weaponBltsTypeDict[bltType]["magSize"] * weaponTypeDict[wpnType]["magSize"]
    params["size"]               *= weaponBltsTypeDict[bltType]["bulletSize"] * weaponTypeDict[wpnType]["bulletSize"]
    params["howManyProjItShoot"] *= weaponBltsTypeDict[bltType]["projItShoot"] * weaponTypeDict[wpnType]["projItShoot"]
    params["howManyProjItTakes"] *= weaponBltsTypeDict[bltType]["projItTakes"] * weaponTypeDict[wpnType]["projItTakes"]
    params["reloadingTime"]      *= weaponBltsTypeDict[bltType]["reloadingTime"] * weaponTypeDict[wpnType]["reloadingTime"]
    params["recoil"]             *= weaponBltsTypeDict[bltType]["recoil"] * weaponTypeDict[wpnType]["recoil"]
    params["minSpeed"]           *= weaponBltsTypeDict[bltType]["minSpeed"] * weaponTypeDict[wpnType]["minSpeed"]
    params["maxSpeed"]           *= weaponBltsTypeDict[bltType]["maxSpeed"] * weaponTypeDict[wpnType]["maxSpeed"]     
def generateTerrainEffects(params):
    randomTable = TerrainEffectsRndTbl
    if params["wpnType"] == "rocketLauncher":
        randomTable = "X" * 40 + "E" * 20 + "F" * 20 + "T" * 20
    loss = random.choice(randomTable)
    if loss == "X":
        params["explBlts"] = 1
        generateTerrainRadius(params)
    elif loss == "E":
        params["elecBlts"] = 1
        generateTerrainRadius(params)
    elif loss == "F":
        params["fireBlts"] = 1
        generateTerrainRadius(params)
    elif loss == "T":
        params["toxicBlts"] = 1
        generateTerrainRadius(params)
def generateCommonEffects(params):
    # wylosowanie upakowanych kulek
    tbl = packedBulletsRndTbl
    if params["wpnType"] == "Shotgun":
        tbl = "1" * 85 + "0" * 15
    loss = random.choice(tbl)
    if loss == "1":
        params["packedBullets"] = 1

    # wylosowanie celownika 
    loss = random.choice(commonEffectsRndTbl)
    if loss == "1":
        params["scope"] = 1

    # wylosowanie stabilizatora
    loss = random.choice(commonEffectsRndTbl)
    if loss == "1":
        params["stabilizator"] = 1

    # wylosowanie powiękoszego magazynka 
    loss = random.choice(commonEffectsRndTbl)
    if loss == "1":
        params["extendedMag"] = 1
def generateAwesomeEffects(params):
    loss = random.choice(awesomeEffectsRndTbl)
    if loss == "I":
        params["smartBlts"] = 1
    elif loss == "M":
        params["multipler"] = 1
    elif loss == "F":
        params["flexibleBlts"] = 1
    elif loss == 'S':
        params["stunEnemies"] = 1
def generateTerrainRadius(params):
    loss = random.choice(terrainRadiusRndTbl)
    params["radiusSize"] = random.randint(int(params["size"]),3*int(params["size"])) * int(loss)
def generateWpnTypeEffects(params):
    if params["wpnType"] == "lead":
        loss = random.choice(leadWpnTypeRndTbl)
        if loss == "G":
            params["grenadeLauncher"] = 1
        elif loss == "F":
            params["thrower"] = 1
    
    elif params["wpnType"] ==  "energetic":
        loss = random.choice(energeticWpnTypeRndTbl)
        if loss == "C":
            params["charger"] = 1
        elif loss == "A":
            params["shootArround"] = 1

    elif params["wpnType"] == "plasma":
        loss = random.choice(plasmaWpnTypeRndTbl)
        if loss == "M":
            params["magicPlasmaBlts"] = 1
        elif loss == "P":
            params["preciseMultiShoot"] = 1
def generateWeaponDict():
    dict = {"name":None, "cost":None, "minDmg":None, "maxDmg":None, "firerate":None,"magSize":None, "recoil":None,"minSpeed":None, "maxSpeed":None,"size":None,"howManyProjItShoot":None,
            "howManyProjItTakes":None, "reloadingTime":None,"wpnType":None,"fireBlts":None,"elecBlts":None,"explBlts":None,"toxicBlts":None,"extendedMag":None,"flexibleBlts":None,"multipler":None,
            "packedBullets":None,"scope":None,"smartBlts":None,"stabilizator":None, "radiusSize":None, "stunEnemies":None, "charger":None, "shootArround":None, "grenadeLauncher":None, "thrower":None,
           "magicPlasmaBlts":None, "preciseMultiShoot":None}
    return dict
def genRandomEffects(params):
    generateTerrainEffects(params)
    generateCommonEffects(params)
    generateAwesomeEffects(params)
    generateWpnTypeEffects(params)
def randomWeapon(args):
    wpnFile = open("generatedWeaponOutput.txt","w")
    for i in range(args.num):
        print ("[*] Generuję broń ",i+1,"/",args.num)
        parameters = generateWeaponDict()
       
        bltType = random.choice(list(weaponBltsTypeDict.keys()))
        wpnType = random.choice(list(weaponTypeDict.keys()))

        parameters["wpnType"] = bltType

        generatePrimaryParams(parameters)
        genRandomEffects(parameters)
        applyConstantsValues(parameters, bltType, wpnType)
        correctParams(parameters)

        wpnFile.write("*   generated " + str(bltType) + " " + str(wpnType) + "\n")
        printToFile(parameters,wpnFile)
###################################################################
####################################################################################################################################################################


####################################################################################################################################################################
# Przeciwnicy 
enemyDict = {	"FLIGHTY" :{ "SIModel":0, "minDamage":low,"maxDamage":low, "maxSpeedX":good, "maxSpeedY":good, "accelerationX":rather_good, 
                          "accelerationY":rather_good, "circleRad":rather_good, "minHP":high, "maxHP":high,"minDefense": high, "maxDefense":high, "minAgility":low, "maxAgility":low, 
                           "viewSize":good, "worth": normal , "reactionTime":good,"safeDistance":high, "minSafeDistance":good,
                            "chargeDistance":low, "attackRange":normal , "boidPith":high, "ChanceOfGettingRandomized":good,"minSizeOfGroup":good, "maxSizeOfGroup":high,
                            "rangeWeaponParams":{} },

	            "AGRESSIVE":{"SIModel":1,"minDamage":high,"maxDamage":high, "maxSpeedX":high, "maxSpeedY":high, "accelerationX":rather_good, "accelerationY":rather_good, "circleRad":rather_good,
                            "minHP":normal, "maxHP":rather_good,"minDefense": low, "maxDefense":average, "minAgility":rather_good, "maxAgility":rather_good,
                           "viewSize":high, "worth": rather_good,"reactionTime":low,"safeDistance":high, "minSafeDistance":high,
                            "chargeDistance":high, "attackRange":good, "boidPith":average, "ChanceOfGettingRandomized":high,"minSizeOfGroup":average, "maxSizeOfGroup":rather_good,
                            "rangeWeaponParams":{} },		

	            "HEAVY":{"SIModel":2, "minDamage":high,"maxDamage":high,"maxSpeedX":low, "maxSpeedY":average, "accelerationX":low, "accelerationY":low, "circleRad":high,
                            "minHP":very_high, "maxHP":very_high,"minDefense": high, "maxDefense":very_high, "minAgility":average, "maxAgility":average,
                           "viewSize":good, "worth": high,"reactionTime":very_high,"safeDistance":high, "minSafeDistance":good,
                            "chargeDistance":average, "attackRange":rather_good, "boidPith":very_low, "ChanceOfGettingRandomized":normal,"minSizeOfGroup":low, "maxSizeOfGroup":low,
                            "rangeWeaponParams":{} },			
	
	            "RUNNERS":{"SIModel":3, "minDamage":average,"maxDamage":rather_good,"maxSpeedX":high, "maxSpeedY":high, "accelerationX":high, "accelerationY":high, "circleRad":average,
                            "minHP":low, "maxHP":average,"minDefense": low, "maxDefense":average, "minAgility":high, "maxAgility":high, 
                            "viewSize":high, "worth": low,"reactionTime":low,"safeDistance":normal, "minSafeDistance":good,
                            "chargeDistance":high, "attackRange":rather_good, "boidPith":high, "ChanceOfGettingRandomized":high,"minSizeOfGroup":high, "maxSizeOfGroup":high,
                            "rangeWeaponParams":{} },		

	            "BEAST":{"SIModel":4,"minDamage":high,"maxDamage":very_high, "maxSpeedX":very_low, "maxSpeedY":high, "accelerationX":low, "accelerationY":low, "circleRad":very_high,
                            "minHP":very_high, "maxHP": (very_high + enormous)/2.,"minDefense": high, "maxDefense":very_high, "minAgility":very_low, "maxAgility":very_low, 
                            "viewSize":high, "worth": enormous,"reactionTime":average,"safeDistance":high, "minSafeDistance":high,
                            "chargeDistance":high, "attackRange":high, "boidPith":really_shitty, "ChanceOfGettingRandomized":low,"minSizeOfGroup":really_shitty, "maxSizeOfGroup":really_shitty,
                            "rangeWeaponParams":{} },			

	            "PREDATOR":{"SIModel":5,"minDamage":good,"maxDamage":very_high, "maxSpeedX":good, "maxSpeedY":good, "accelerationX":very_high, "accelerationY":very_high, "circleRad":rather_good,
                            "minHP":low, "maxHP":very_high,"minDefense": low, "maxDefense":rather_good, "minAgility":high, "maxAgility":very_high, 
                            "viewSize":very_high, "worth": high,"reactionTime":almost_none,"safeDistance":high, "minSafeDistance":good,
                            "chargeDistance":very_high, "attackRange":high, "boidPith":average, "ChanceOfGettingRandomized":average,"minSizeOfGroup":low, "maxSizeOfGroup":good,
                            "rangeWeaponParams":{} },		
	
	            "BITTERS":{"SIModel":6,"minDamage":very_low,"maxDamage":low, "maxSpeedX":very_high, "maxSpeedY":very_high, "accelerationX":rather_good, "accelerationY":rather_good, "circleRad":low,
                            "minHP":almost_none, "maxHP":low,"minDefense": almost_none, "maxDefense":low, "minAgility":high, "maxAgility":high, 
                            "viewSize":good, "worth": low,"reactionTime":almost_none,"safeDistance":rather_good, "minSafeDistance":good,
                            "chargeDistance":high, "attackRange":good, "boidPith":average, "ChanceOfGettingRandomized":rather_good,"minSizeOfGroup":high, "maxSizeOfGroup":very_high,
                            "rangeWeaponParams":{} },		

	            "ELEMENTS":{"SIModel":7,"minDamage":average,"maxDamage":high, "maxSpeedX":average, "maxSpeedY":average, "accelerationX":rather_good, "accelerationY":rather_good, "circleRad":rather_good,
                            "minHP":high, "maxHP":high,"minDefense": high, "maxDefense":high,  "minAgility":low, "maxAgility":low,
                            "viewSize":good, "worth": normal,"reactionTime":rather_good,"safeDistance":high, "minSafeDistance":good,
                            "chargeDistance":low, "attackRange":normal , "boidPith":high, "ChanceOfGettingRandomized":high,"minSizeOfGroup":good, "maxSizeOfGroup":high,
                            "rangeWeaponParams":{} },		
};
enemyRandomValues = {"minDamage":[10,40],"maxDamage":[50,80],"maxSpeedX":[100,200], "maxSpeedY":[200,300], "accelerationX":[30,40], "accelerationY":[30,40], "circleRad":[6,12],
                            "minHP":[80,120], "maxHP":[120,200],"minDefense": [30,50], "maxDefense":[50,60], "minAgility":[60,70],
                           "maxAgility":[20,30], "viewSize":[300,400], "worth": [20,40], "reactionTime":[300,400],"safeDistance":[500,600],
                          "minSafeDistance":[250,350], "chargeDistance":[200,300], "attackRange":[60,80], "boidPith":[200,300], "ChanceOfGettingRandomized":[40,60],
                          "minSizeOfGroup":[2,5], "maxSizeOfGroup":[8,13] }


###################################################################
# Funkcje do Przeciwników
def randomEnemy(args):
    enemyFile = open("enemyGeneratedOutput.txt","w")

    for i in range(args.num):
        print("[*] Generuję przeciwnika ",i+1)
        params = generateEmptyEnemyParams()
        template = random.choice(list(enemyDict.keys()))
        
        params["nameOfSIModel"] = template
        params["SIModel"] = 1
        generateEnemyPrimParams(params)
        manageEnemyParamters(params)
        correctEnemyParameters(params)

        enemyFile.write("* Wygenerowano " + template)
        printToFile(params,enemyFile)
def generateEnemyPrimParams(params):
    for value in enemyRandomValues:
        params[value] = random.randint(enemyRandomValues[value][0],enemyRandomValues[value][1])
def generateEmptyEnemyParams():
    Dict = {"nameOfSIModel":None,"SIModel":None,"minDamage":None,"maxDamage":None, "maxSpeedX":None, "maxSpeedY":None, "accelerationX":None, "accelerationY":None, "circleRad":None,
                            "minHP":None, "maxHP":None,"minDefense": None, "maxDefense":None, "minAgility":None, "maxAgility":None, 
                           "viewSize":None, "worth": None, "reactionTime":None,"safeDistance":None, "minSafeDistance":None,
                            "chargeDistance":None, "attackRange":None, "boidPith":None, "ChanceOfGettingRandomized":None,"minSizeOfGroup":None, "maxSizeOfGroup":None,
                            "rangeWeaponParams":{} }
    return	Dict
def manageEnemyParamters(params):
    vals = dict(enemyDict[params["nameOfSIModel"]])

    for value in list(vals)[:-1]:
        params[value] = int(params[value] * vals[value])
def correctEnemyParameters(params):
    
    params["maxSpeedX"]   = max(params["maxSpeedX"],params["maxSpeedY"])
    params["maxSpeedY"]  = params["maxSpeedX"] 

    params["accelerationX"] = max(params["accelerationX"],params["accelerationY"])
    params["accelerationY"] = params["accelerationX"]
    params["circleRad"]     = max(3, params["circleRad"])

    params["minHP"],params["maxHP"]                   = correctMinMax(params["minHP"],params["maxHP"])
    params["minDefense"],params["maxDefense"]         = correctMinMax(params["minDefense"],params["maxDefense"])
    params["minAgility"], params["maxAgility"]        = correctMinMax(params["minAgility"], params["maxAgility"])
    params["safeDistance"]                            = min (1000,params["safeDistance"]) 
    params["minSafeDistance"], params["safeDistance"] = correctMinMax(params["minSafeDistance"], params["safeDistance"])

    params["minSizeOfGroup"]                          = max(1, params["minSizeOfGroup"])
    params["maxSizeOfGroup"]                          = max(1, params["maxSizeOfGroup"])
    params["minSizeOfGroup"],params["maxSizeOfGroup"] = correctMinMax(params["minSizeOfGroup"],params["maxSizeOfGroup"])

    params["minDamage"]                               = max(1, params["minDamage"])
    params["maxDamage"]                               = max(1, params["maxDamage"])
    params["minDamage"],params["maxDamage"]           = correctMinMax(params["minDamage"],params["maxDamage"])

def correctMinMax(minV, maxV):
    buff = max(minV,maxV)
    minV = min(minV, maxV)
    maxV = buff
    return minV, maxV

###################################################################
####################################################################################################################################################################

 # TODO 
 # dodac system poziomow :

def main():
    parser = argparse.ArgumentParser(description="Simple Script that generate random weapon or random enemy for my game, Splatter")
    parser.add_argument("type", type=str,help="Type of generated Output ;\n 'weapon': generate random weapon, 'enemy' : generate random enemy")
    parser.add_argument("num",type=int, help="How many object it generates")
    parser.add_argument("-l","--level",type=int,help="level of awesomeness : (0,1> - normal weapon/enemy, <1,2) - great weapon/enemy, <2,3) - greater weapon/enemy, <3,inf) - really unbalanced weapon/enemy")
    args = parser.parse_args()

    if args.type == "weapon":
        randomWeapon(args)
    else:
        randomEnemy(args)


if __name__ == "__main__":
    main()

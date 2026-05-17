#define MAX_GATESS 70
#define INVALID_GATE_ID -1

#define OPEN_GATE 17

#define GetGateData(%0,%1)    GateInfo[%0][%1]
#define SetGateData(%0,%1,%2) GateInfo[%0][%1] = %2

enum E_GATE_DATA
{
    GateSQL_ID,
    Gate_type,
    Float: Gate1_X,
    Float: Gate1_Y,
    Float: Gate1_Z,
    Float: Gate1_Angle,
    Float: Gate2_X,
    Float: Gate2_Y,
    Float: Gate2_Z,
    Float: Gate2_Angle,
    Float: GateZone_X,
    Float: GateZone_Y,
    Float: GateZone_Z,
    DynamicObject: GateObj1_ID,
    DynamicObject: GateObj2_ID,
    DynamicArea: GateArea_ID,
    GateIsOpened,
    GateNearestHouseID
}

new GateInfo[MAX_GATESS][E_GATE_DATA];
new g_gates_count = 0;

// это массив хранящий координаты для добавления в бд
// если надо поменять координаты каких-либо ворот, то это надо делать в бд
new const Float:InitialGateCoords[][12] =
{
    {0, -972.25,266.04,26.16,-35.0, -977.99,269.97,26.16,147.0, -975.109802,268.110626,25.564952},
    {0, -981.31,207.61,25.42,56.00, -985.21,201.90,25.42,-126.0, -983.656860,204.917953,24.822523},
    {1, -1087.87,268.21,25.56,143.2, 0,     0,     0,    0,      -1085.882812,267.158691,24.968379}, // 1 дверь
    {0, -1014.42,199.56,26.37,147.0, -1008.59,195.75,26.37,-33.0, -1011.150085,197.888442,25.770933},
    {0, -1050.70,296.34,25.72,-34.0, -1056.42,300.26,25.72,146.0, -1053.922973,297.880523,25.113557},
    {0, -1138.42,451.45,21.36,145.0, -1132.72,447.48,21.36,-35.0, -1135.271118,449.727020,20.760240},
    {1, -1205.47,547.98,18.81,118.0,  0,      0,     0,     0,    -1204.079223,546.164978,18.209592}, // 1 дверь
    {0, -1262.13,650.35,17.54,119.0, -1258.73,644.24,17.54,-61.0, -1260.025756,647.346618,16.921714},
    {0, -1297.32,708.67,17.41,121.0, -1293.63,702.65,17.41,-58.0, -1295.061035,705.669860,16.802509},
    {0, -1258.29,275.92,33.81,176.0, -1251.21,275.41,33.81,-4.0,  -1254.857299,275.269348,33.212692},
    {0, -1308.53,310.21,33.58,145.0, -1302.85,306.26,33.58,-34.0, -1306.021972,307.950347,32.984519},
    {0, -1356.85,374.00,33.50,124.0, -1352.94,368.20,33.50,-56.0, -1354.431762,371.075744,32.877708},
    {0, -1306.15,386.45,33.16,-54.0, -1310.26,392.01,33.16,127.0, -1308.610229,389.161682,32.559524},
    {0, -1389.25,423.23,32.55,121.0, -1385.70,417.31,32.55,-59.0, -1387.168457,420.497558,31.938650},
    {0, -1328.10,419.58,32.08,-59.0, -1331.62,425.54,32.08,120.0, -1330.218750,422.353057,31.478160},
    {0, -1416.20,468.69,31.79,121.0, -1412.66,462.75,31.79,-60.0, -1414.110961,466.003356,31.181919},
    {0, -1369.23,488.98,31.91,-59.0, -1372.83,494.90,31.91,121.0, -1371.352539,491.736877,31.314098},
    {1, -1458.09,542.42,31.78,121.0, 0,       0,     0,    0,     -1456.657226,540.784912,31.181507}, // 1 дверь
    {0, -1411.62,559.31,31.87,-59.0, -1415.23,565.22,31.87,121.0, -1413.757568,562.035766,31.267444},
    {0, -1518.83,643.05,32.17,121.0, -1515.22,637.15,32.17,-59.0, -1516.688354,640.314086,31.568733},
    {0, -1467.64,652.09,31.58,-59.0, -1471.18,658.03,31.58,120.0, -1469.785034,654.850646,30.963838},
    {0, -1563.82,698.53,32.17,126.0, -1560.00,693.19,31.57,-55.0, -1561.476196,695.936523,31.574047},
    {0, -1616.48,722.02,33.45,65.0,  -1619.40,715.82,33.45,-115.0,-1617.591186,718.762145,32.851882},
    {0, -1943.93,650.19,29.71,-77.0, -1945.47,656.91,29.71,103.0, -1945.084960,653.502441,29.099472},
    {0, -1978.35,630.05,29.51,104.0, -1976.71,623.32,29.51,-77.0, -1977.140747,626.770935,28.894866},
    {1, -1920.43,564.34,30.43,-72.0, 0,       0,     0,    0,     -1921.453247,566.277038,29.834266}, // 1 дверь
    {0, -1960.20,519.30,29.51,13.0,  -1966.93,517.66,29.51,-166.0,-1963.491210,518.151977,28.915008},
    {0, -1879.32,463.31,29.14,-64.0, -1882.41,469.50,29.14,117.0, -1881.160278,466.211700,28.531690},
    {0, -1983.90,458.28,30.40,-153.0,-1977.80,461.39,30.40,27.0,  -1981.020019,460.183502,29.796112},
    {0, -1859.15,420.81,29.32,-66.0, -1862.11,427.05,29.32,116.0, -1860.947387,423.834442,28.703405},
    {0, -1886.51,383.35,28.61,116.0, -1883.47,377.19,28.61,-63.0, -1884.639526,380.324066,28.014749}
};

stock GetNearestGate(playerid)
{
    new Float:playerX, Float:playerY, Float:playerZ;
    GetPlayerPos(playerid, playerX, playerY, playerZ);

    new Float:minDistance = 100.0;
    new nearestGateID = INVALID_GATE_ID;

    for (new i = 0; i < g_gates_count; i++)
    {
        new Float:gateZoneX = GetGateData(i, GateZone_X);
        new Float:gateZoneY = GetGateData(i, GateZone_Y);
        new Float:gateZoneZ = GetGateData(i, GateZone_Z);

        new Float:distance = floatsqroot(floatpower(playerX - gateZoneX, 2.0) + floatpower(playerY - gateZoneY, 2.0) + floatpower(playerZ - gateZoneZ, 2.0));

        if(distance < minDistance)
        {
            minDistance = distance;
            nearestGateID = i;
        }
    }
    return nearestGateID;
}

public OnGameModeInit()
{
    SetTimer("CheckAndLoadGates", 2000, false);
    SetTimer("LoadGates", 5000, false);
    
    #if defined gate_OnGameModeInit
        return gate_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit gate_OnGameModeInit
#if defined gate_OnGameModeInit
    forward gate_OnGameModeInit();
#endif

forward CheckAndLoadGates();
public CheckAndLoadGates()
{
    new Cache:cache;
    new query[512];
    
    mysql_format(mysql, query, sizeof(query), "SELECT 1 FROM `gates` LIMIT 1");
    cache = mysql_query(mysql, query, true);
    
    if(mysql_errno() != 0)
    {
        printf("[GATES] Таблица 'gates' не найдена. Создаем новую структуру...");
        mysql_format(mysql, query, sizeof(query), "CREATE TABLE `gates` (" \
            "`id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY," \
            "`gate_type` INT NOT NULL," \
            "`gate1_x` FLOAT NOT NULL," \
            "`gate1_y` FLOAT NOT NULL," \
            "`gate1_z` FLOAT NOT NULL," \
            "`gate1_angle` FLOAT NOT NULL," \
            "`gate2_x` FLOAT NOT NULL," \
            "`gate2_y` FLOAT NOT NULL," \
            "`gate2_z` FLOAT NOT NULL," \
            "`gate2_angle` FLOAT NOT NULL," \
            "`gatezone_x` FLOAT NOT NULL," \
            "`gatezone_y` FLOAT NOT NULL," \
            "`gatezone_z` FLOAT NOT NULL" \
            ") ENGINE = InnoDB");
        
        mysql_query(mysql, query, false);
        
        if(mysql_errno() != 0)
        {
            printf("[GATES] Ошибка при создании таблицы 'gates': %d", mysql_errno());
            return 0;
        }
        else
        {
            printf("[GATES] Таблица 'gates' успешно создана.");
        }

        printf("[GATES] Вставляем начальные данные в новую таблицу...");
        for(new i = 0; i < sizeof(InitialGateCoords); i++)
        {
            mysql_format(mysql, query, sizeof(query), "INSERT INTO `gates` (" \
                "gate_type, gate1_x, gate1_y, gate1_z, gate1_angle, gate2_x, gate2_y, gate2_z, gate2_angle, gatezone_x, gatezone_y, gatezone_z" \
                ") VALUES (" \
                "'%d','%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f'" \
                ")", 
                InitialGateCoords[i][0], InitialGateCoords[i][1], InitialGateCoords[i][2], InitialGateCoords[i][3], InitialGateCoords[i][4],
                InitialGateCoords[i][5], InitialGateCoords[i][6], InitialGateCoords[i][7], InitialGateCoords[i][8],
                InitialGateCoords[i][9], InitialGateCoords[i][10], InitialGateCoords[i][11]
            );
            mysql_query(mysql, query, false);
        }
        printf("[GATES] Начальные данные успешно вставлены.");
    }
    cache_delete(cache);
}

forward LoadGates();
public LoadGates()
{
    new Cache:cache = mysql_query(mysql, "SELECT * FROM `gates`", true);
    g_gates_count = cache_num_rows(cache);

    new GATES_VOR1, GATES_VOR2;
    
    if(g_gates_count > 0)
    {
        for (new i = 0; i < g_gates_count; i++)
        {
            SetGateData(i, GateSQL_ID, cache_get_field_content_int(i, "id"));
            SetGateData(i, Gate_type, cache_get_field_content_float(i, "gate_type"));
            SetGateData(i, Gate1_X, cache_get_field_content_float(i, "gate1_x"));
            SetGateData(i, Gate1_Y, cache_get_field_content_float(i, "gate1_y"));
            SetGateData(i, Gate1_Z, cache_get_field_content_float(i, "gate1_z"));
            SetGateData(i, Gate1_Angle, cache_get_field_content_float(i, "gate1_angle"));
            SetGateData(i, Gate2_X, cache_get_field_content_float(i, "gate2_x"));
            SetGateData(i, Gate2_Y, cache_get_field_content_float(i, "gate2_y"));
            SetGateData(i, Gate2_Z, cache_get_field_content_float(i, "gate2_z"));
            SetGateData(i, Gate2_Angle, cache_get_field_content_float(i, "gate2_angle"));
            SetGateData(i, GateZone_X, cache_get_field_content_float(i, "gatezone_x"));
            SetGateData(i, GateZone_Y, cache_get_field_content_float(i, "gatezone_y"));
            SetGateData(i, GateZone_Z, cache_get_field_content_float(i, "gatezone_z"));
            
            SetGateData(i, GateIsOpened, 0);
            SetGateData(i, GateNearestHouseID, -1);

            if(GetGateData(i, Gate_type) <= 0)
            {
                GATES_VOR1 = CreateDynamicObject(7241, GetGateData(i, Gate1_X), GetGateData(i, Gate1_Y), GetGateData(i, Gate1_Z), 0.0, 0.0, GetGateData(i, Gate1_Angle));
                GATES_VOR2 = CreateDynamicObject(7241, GetGateData(i, Gate2_X), GetGateData(i, Gate2_Y), GetGateData(i, Gate2_Z), 0.0, 0.0, GetGateData(i, Gate2_Angle));
            }
            else
            {
                GATES_VOR1 = CreateDynamicObject(7242, GetGateData(i, Gate1_X), GetGateData(i, Gate1_Y), GetGateData(i, Gate1_Z), 0.0, 0.0, GetGateData(i, Gate1_Angle));
                GATES_VOR2 = INVALID_GATE_ID;
            }

            SetGateData(i, GateObj1_ID, GATES_VOR1);
            SetGateData(i, GateObj2_ID, GATES_VOR2);
            SetGateData(i, GateArea_ID, CreateDynamicSphere(GetGateData(i, GateZone_X), GetGateData(i, GateZone_Y), GetGateData(i, GateZone_Z), 5.0));

            new Float:gateZoneX = GetGateData(i, GateZone_X);
            new Float:gateZoneY = GetGateData(i, GateZone_Y);
            new Float:gateZoneZ = GetGateData(i, GateZone_Z);
            
            new nearestHouseID_temp = -1;
            new Float:minDistance_temp = 100.0;
            new totalHouses = MAX_HOUSES;
            
            for (new h = 0; h < totalHouses; h++)
            {
                new Float:houseX = GetHouseData(h, H_POS_X);
                new Float:houseY = GetHouseData(h, H_POS_Y);
                new Float:houseZ = GetHouseData(h, H_POS_Z);

                if(houseX != 0.0 || houseY != 0.0 || houseZ != 0.0)
                {
                    new Float:distance = floatsqroot(floatpower(gateZoneX - houseX, 2.0) + floatpower(gateZoneY - houseY, 2.0) + floatpower(gateZoneZ - houseZ, 2.0));

                    if(distance < minDistance_temp)
                    {
                        minDistance_temp = distance;
                        nearestHouseID_temp = h;
                    }
                }
            }
            SetGateData(i, GateNearestHouseID, nearestHouseID_temp);
        }
    }
    cache_delete(cache);
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
    new ownerFamilyRang, ownerFamilyId, playerFamilyId;
    for(new i = 0; i < g_gates_count; i++)
    {
        if(areaid == GetGateData(i, GateArea_ID))
        {
            new playerHouseID = GetPlayerHouse(playerid);

            //if(playerHouseID == 0) return 1;

            new gateNearestHouse = GetGateData(i, GateNearestHouseID);

            if(gateNearestHouse != -1)
            {
                new houseOwnerID = GetHouseData(gateNearestHouse, H_OWNER_ID);

                if(houseOwnerID == GetPlayerAccountID(playerid))
                {
                    ShowNewNotification(playerid, 4, 5, OPEN_GATE, 1, "Открыть ворота", "");
                    ShowNotification(playerid, 4, "Открыть ворота", 3, "/opengate", ">>");
                    return 1;
                }

                if(houseOwnerID > 0)
                {      
                    new query[256], Cache:result;

                    mysql_format(mysql, query, sizeof(query), "SELECT `family_rang`, `family_id` FROM `accounts` WHERE `id` = %d", houseOwnerID);
                    result = mysql_query(mysql, query);

                    ownerFamilyRang = cache_get_field_content_int(0, "family_rang");
                    ownerFamilyId = cache_get_field_content_int(0, "family_id");

                    cache_delete(result);

                    if(ownerFamilyRang == 5)
                    {
                        mysql_format(mysql, query, sizeof(query), "SELECT `family_id` FROM `accounts` WHERE `id` = %d", GetPlayerAccountID(playerid));
                        mysql_query(mysql, query);

                        playerFamilyId = cache_get_field_content_int(0, "family_id");

                        cache_delete(result);

                        if(playerFamilyId == ownerFamilyId)
                        {
                            ShowNewNotification(playerid, 4, 5, OPEN_GATE, 1, "Открыть ворота", "");
                            ShowNotification(playerid, 4, "Открыть ворота", 3, "/opengate", ">>");
                        }
                        else
                        {
                            // Игрок не в семье в которой состоит владелец дома
                            return 1;
                        }
                    }
                    else
                    {
                        // Ранг владельца не 5
                        return 1;
                    }
                }
                else
                {
                    // У дома нет владельца
                    return 1;
                }
            }
            else return 1;
        }
    }
    #if defined gate_OnPlayerEnterDynamicArea
        return gate_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea gate_OnPlayerEnterDynamicArea
#if defined gate_OnPlayerEnterDynamicArea
    forward gate_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
#endif

stock GetNearestGateByPlayer(playerid)
{
    new Float:targetX, Float:targetY, Float:targetZ;

    if(IsPlayerInAnyVehicle(playerid))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        GetVehiclePos(vehicleid, targetX, targetY, targetZ);
    }
    else GetPlayerPos(playerid, targetX, targetY, targetZ);
    
    new Float:minDistance = 5.0;
    new nearestGateID = INVALID_GATE_ID;

    for (new i = 0; i < g_gates_count; i++)
    {
        new Float:gateZoneX = GetGateData(i, GateZone_X);
        new Float:gateZoneY = GetGateData(i, GateZone_Y);
        new Float:gateZoneZ = GetGateData(i, GateZone_Z);

        new Float:distance = floatsqroot(floatpower(targetX - gateZoneX, 2.0) + floatpower(targetY - gateZoneY, 2.0) + floatpower(targetZ - gateZoneZ, 2.0));

        if(distance < minDistance)
        {
            minDistance = distance;
            nearestGateID = i;
        }
    }
    return nearestGateID;
}

CMD:checkgate(playerid, params[])
{
    new nearestGateID = GetNearestGateByPlayer(playerid);

    if(nearestGateID != INVALID_GATE_ID)
    {
        new sqlID = GetGateData(nearestGateID, GateSQL_ID);
        new message[128];
        
        format(message, sizeof(message), "Вы находитесь рядом с воротами. Серверный ID: %d, SQL ID: %d.", nearestGateID, sqlID);
        SendClientMessage(playerid, 0xFFFFFFFF, message);
    }
    else
    {
        SendClientMessage(playerid, 0xFFFFFFFF, "Рядом с вами нет ворот либо Вы далеко.");
    }
    return 1;
}

public CloseGateTimer(playerid, gateid, Float:gateVor1X, Float:gateVor1Y, Float:gateVor1Z, Float:gateVor1Angel, Float:gateVor2X, Float:gateVor2Y, Float:gateVor2Z, Float:gateVor2Angel)
{
    MoveDynamicObject(GetGateData(gateid, GateObj1_ID), gateVor1X, gateVor1Y, gateVor1Z, 7.0, 0, 0, gateVor1Angel);
    MoveDynamicObject(GetGateData(gateid, GateObj2_ID), gateVor2X, gateVor2Y, gateVor2Z, 7.0, 0, 0, gateVor2Angel);

    // Сбрасываем флаг открытия ворот
    DeletePVar(playerid, "GateOpening");

    return 1;
}

CMD:opengate(playerid)
{
    new gateid = GetNearestGateByPlayer(playerid);
 
    printf("Полученный айди в GetNearestGateByPlayer = %d", gateid);
    printf("Айди ворот при открытии %d", GetGateData(gateid, GateObj1_ID));

    if(gateid != INVALID_GATE_ID)
    {
        new Float:gateVor1X = GetGateData(gateid, Gate1_X);
        new Float:gateVor1Y = GetGateData(gateid, Gate1_Y);
        new Float:gateVor1Z = GetGateData(gateid, Gate1_Z);
        new Float:gateVor1Angel = GetGateData(gateid, Gate1_Angle);

        new Float:gateVor2X = GetGateData(gateid, Gate2_X);
        new Float:gateVor2Y = GetGateData(gateid, Gate2_Y);
        new Float:gateVor2Z = GetGateData(gateid, Gate2_Z);
        new Float:gateVor2Angel = GetGateData(gateid, Gate2_Angle);

        MoveDynamicObject(GetGateData(gateid, GateObj1_ID), gateVor1X, gateVor1Y, gateVor1Z, 7.0, 0, 0, gateVor1Angel - 90);
        MoveDynamicObject(GetGateData(gateid, GateObj2_ID), gateVor2X, gateVor2Y, gateVor2Z, 7.0, 0, 0, gateVor2Angel + 90);

        SetTimerEx("CloseGateTimer", 5000, false, "iiffffffff", playerid, gateid, gateVor1X, gateVor1Y, gateVor1Z, gateVor1Angel, gateVor2X, gateVor2Y, gateVor2Z, gateVor2Angel);
    }
    else print("Ошибка в открытии ворот INVALID_GATE_ID");
}

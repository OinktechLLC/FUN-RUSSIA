// Ко всем #define
#define DIALOG_HELPVEH 1200

// В конец мода
CMD:tune(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_HELPVEH, DIALOG_STYLE_LIST,
        "Помощь по транспортным командам",
"/toner — Выдать тонировку\n\
/nitro — Выдать нитро\n\
/lc — Выдать Launch Control\n\
/hydra — Выдать гидравлику",
        "Выбрать", "Закрыть");
       
       

    new vehicleid = GetPlayerVehicleID(playerid);
    if(vehicleid == INVALID_VEHICLE_ID)
        return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Не удалось получить ID автомобиля!");


    // Проверка: это личный транспорт?
    if(GetVehicleData(vehicleid, V_ACTION_TYPE) != VEHICLE_ACTION_TYPE_OWNABLE_CAR)
        return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Это не ваш автомобиль!");

    // Проверка: принадлежит ли игроку
    new owner_id = GetVehicleData(vehicleid, V_ACTION_ID);
    if(GetOwnableCarData(owner_id, OC_OWNER_ID) != GetPlayerAccountID(playerid))
        return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Это не ваш автомобиль!");

    // Проверка: игрок водитель?
    if(!IsPlayerDriver(playerid))
        return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Вы должны быть водителем!");
        
    return 1;
}



// В if(dialogid)
if(dialogid == DIALOG_HELPVEH)
{
    if(!response) return 1;

    switch(listitem)
    {
        
        case 0: callcmd::toner(playerid, "");
        case 1: callcmd::nitro(playerid, "");
        case 2: callcmd::lc(playerid, "");
        case 3: callcmd::hydra(playerid, "");
    }
    return 1;
}

// В конец

CMD:hydra(playerid, params[])
 {
     if(!IsPlayerInAnyVehicle(playerid))
      return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Вы не в транспорте!");

   new vehicleid = GetPlayerVehicleID(playerid);
   SetVehicleHydraulics(vehicleid, 1, 1);
    SendClientMessage(playerid, -1, "{FF5252}Гидравлика: {FFFFFF}Активирована (тип: 1)!");
  return 1;
 }

CMD:toner(playerid, params[])
{
    new vehicleid = GetPlayerVehicleID(playerid);
    if(!vehicleid) return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Вы не в транспорте!");

    new front = 0xFF000000;  
    new rear = 0xFF000000;
    new front_side = 0xFF000000;
    new rear_side = 0xFF000000;

    SetVehicleToner(vehicleid, front, rear, front_side, rear_side, 1);
    SendClientMessage(playerid, -1, "{FF5252}Тонер: {FFFFFF}Установлен (чёрный)!");
    return 1;
}

CMD:nitro(playerid, params[])
{
    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "Вы должны быть в машине!");

    new vehicleid = GetPlayerVehicleID(playerid);

    AddVehicleComponent(vehicleid, 1010);
    
	
    
	return 1;
}

CMD:lc(playerid, params[])
 {
 if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{FF0000}Ошибка: {FFFFFF}Вы не в транспорте!");

     new vehicleid = GetPlayerVehicleID(playerid);
  SetVehicleLaunchControl(vehicleid, 1, 1);
   SendClientMessage(playerid, -1, "{FF5252}Launch Control: {FFFFFF}Активирован (тип: 1)!");
    return 1;
 }


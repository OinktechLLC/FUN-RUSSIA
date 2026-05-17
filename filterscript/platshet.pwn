
new PlayerText:Tablet_BG[MAX_PLAYERS];
new PlayerText:Tablet_BTN_GPS[MAX_PLAYERS];
new PlayerText:Tablet_BTN_PLAYERS[MAX_PLAYERS];
new PlayerText:Tablet_BTN_BIZ[MAX_PLAYERS];
new PlayerText:Tablet_BTN_SETTINGS[MAX_PLAYERS];
new bool:TabletOpened[MAX_PLAYERS];

CMD:tabletGUI(playerid)
{
    if(!TabletOpened[playerid]) ShowTablet(playerid);
    else HideTablet(playerid);
    return 1;
}

ShowTablet(playerid)
{
    TabletOpened[playerid] = true;

    Tablet_BG[playerid] = CreatePlayerTextDraw(playerid, 150.0, 100.0, " ");
    PlayerTextDrawFont(playerid, Tablet_BG[playerid], 1);
    PlayerTextDrawLetterSize(playerid, Tablet_BG[playerid], 0.6, 3.0);
    PlayerTextDrawTextSize(playerid, Tablet_BG[playerid], 450.0, 320.0);
    PlayerTextDrawUseBox(playerid, Tablet_BG[playerid], 1);
    PlayerTextDrawBoxColor(playerid, Tablet_BG[playerid], 0x000000AA);
    PlayerTextDrawShow(playerid, Tablet_BG[playerid]);

    Tablet_BTN_GPS[playerid] = CreatePlayerTextDraw(playerid, 170.0, 140.0, "GPS");
    PlayerTextDrawFont(playerid, Tablet_BTN_GPS[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Tablet_BTN_GPS[playerid], 0.3, 1.4);
    PlayerTextDrawColor(playerid, Tablet_BTN_GPS[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetSelectable(playerid, Tablet_BTN_GPS[playerid], true);
    PlayerTextDrawShow(playerid, Tablet_BTN_GPS[playerid]);

    Tablet_BTN_PLAYERS[playerid] = CreatePlayerTextDraw(playerid, 170.0, 180.0, "Игроки");
    PlayerTextDrawFont(playerid, Tablet_BTN_PLAYERS[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Tablet_BTN_PLAYERS[playerid], 0.3, 1.4);
    PlayerTextDrawColor(playerid, Tablet_BTN_PLAYERS[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetSelectable(playerid, Tablet_BTN_PLAYERS[playerid], true);
    PlayerTextDrawShow(playerid, Tablet_BTN_PLAYERS[playerid]);

    Tablet_BTN_BIZ[playerid] = CreatePlayerTextDraw(playerid, 170.0, 220.0, "Бизнес");
    PlayerTextDrawFont(playerid, Tablet_BTN_BIZ[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Tablet_BTN_BIZ[playerid], 0.3, 1.4);
    PlayerTextDrawColor(playerid, Tablet_BTN_BIZ[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetSelectable(playerid, Tablet_BTN_BIZ[playerid], true);
    PlayerTextDrawShow(playerid, Tablet_BTN_BIZ[playerid]);

    Tablet_BTN_SETTINGS[playerid] = CreatePlayerTextDraw(playerid, 170.0, 260.0, "Настройки");
    PlayerTextDrawFont(playerid, Tablet_BTN_SETTINGS[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Tablet_BTN_SETTINGS[playerid], 0.3, 1.4);
    PlayerTextDrawColor(playerid, Tablet_BTN_SETTINGS[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetSelectable(playerid, Tablet_BTN_SETTINGS[playerid], true);
    PlayerTextDrawShow(playerid, Tablet_BTN_SETTINGS[playerid]);

    SelectTextDraw(playerid, 0xFFFFFFAA);
    return 1;
}

HideTablet(playerid)
{
    TabletOpened[playerid] = false;
    CancelSelectTextDraw(playerid);

    PlayerTextDrawHide(playerid, Tablet_BG[playerid]);
    PlayerTextDrawHide(playerid, Tablet_BTN_GPS[playerid]);
    PlayerTextDrawHide(playerid, Tablet_BTN_PLAYERS[playerid]);
    PlayerTextDrawHide(playerid, Tablet_BTN_BIZ[playerid]);
    PlayerTextDrawHide(playerid, Tablet_BTN_SETTINGS[playerid]);
    return 1;
}

public OnPlayerClickPlayerTD(playerid, PlayerText:td)
{
    if(td == Tablet_BTN_GPS[playerid])
    {
        HideTablet(playerid);
        ShowPlayerDialog(playerid, 5000, DIALOG_STYLE_LIST,
        "GPS Навигация",
        "авторынок\работы\начальные работы\вокзалы\ближайшие места\Банк\государственные организации\важные места",
        "Выбрать", "Закрыть");
        return 1;
    }

    if(td == Tablet_BTN_PLAYERS[playerid])
    {
        HideTablet(playerid);
        ShowPlayerDialog(playerid, 5001, DIALOG_STYLE_MSGBOX,
        "Игроки онлайн",
        "Список будет позже.",
        "Ок", "");
        return 1;
    }

    if(td == Tablet_BTN_BIZ[playerid])
    {
        HideTablet(playerid);
        ShowPlayerDialog(playerid, 5002, DIALOG_STYLE_MSGBOX,
        "Бизнесы",
        "Раздел в разработке.",
        "Ок", "");
        return 1;
    }

    if(td == Tablet_BTN_SETTINGS[playerid])
    {
        HideTablet(playerid);
        ShowPlayerDialog(playerid, 5003, DIALOG_STYLE_MSGBOX,
        "Настройки",
        call; CMD:mm/1
        "Ок", "");
        return 1;
    }
    return 0;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == 5000 && response)
    {
        switch(listitem)
        {
            case 0: SetPlayerPos(playerid, 1145.2, -1720.3, 13.4);
            case 1: SetPlayerPos(playerid, 55.2, -83.3, 1.9);
            case 2: SetPlayerPos(playerid, 1740.5, -1862.1, 13.5);
            case 3: SetPlayerPos(playerid, 1400.4, -1720.3, 13.2);
            case 4: SetPlayerPos(playerid, 1250.0, -1450.0, 13.2);
            case 5: SetPlayerPos(playerid, 1470.1, -1300.2, 13.2);
            case 6: SetPlayerPos(playerid, 1550.1, -1670.5, 13.5);
            case 7: SetPlayerPos(playerid, 1600.3, -1800.2, 13.5);
        }
        SendClientMessage(playerid, -1, "✔ Телепортация выполнена.");
        return 1;
    }
    return 1;
}

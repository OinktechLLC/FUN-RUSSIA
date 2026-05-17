#define BrNotification 						13
//#define 28                       28
#define STRMAX 32

stock ShowNotificationNew(playerid, type, duration, id, subId, caption[], btnCaption[])
{
    new Node:JSONObject = JSON_Object();

    JSON_SetInt(JSONObject, "t", type);
    JSON_SetInt(JSONObject, "d", duration);
    JSON_SetInt(JSONObject, "s", id);
    JSON_SetInt(JSONObject, "b", subId);
    JSON_SetString(JSONObject, "i", caption, strlen(caption));
    JSON_SetString(JSONObject, "k", btnCaption, strlen(btnCaption));

    ShowPlayerGUI(playerid, 13, JSONObject);
}

CMD:notify(playerid, params[])
{
    // Параметры уведомления
    new type = 1;         // Тип уведомления (обычно 1 — нормальное уведомление)
    new duration = 5000;  // Время отображения в мс (5000 = 5 секунд)
    new id = 0;           // ID уведомления (можно 0, если не используется)
    new subId = 0;        // subId (тоже можно 0, если не используется)

    ShowNotificationNew(playerid,  5, 6, 0, 0, "Нажмите для взаимодействия!", ">"); // вызов уведомление
    return 1;
}

IPacket:251(playerid, BitStream:bs)
{
    new
        header,
        rpcid,
        data_len,
        data[512],
        outcoming_data[512],
        Node:json = JSON_Object(),
        Node:json_packet_data = JSON_Object();

    BS_ReadValue(bs, PR_UINT8, header);
    BS_ReadValue(bs, PR_UINT8, rpcid);

    BS_ReadValue(bs, PR_UINT16, data_len);
    BS_ReadValue(bs, PR_STRING, data, data_len);

    printf("JSON received: %.*s", data_len, data);
    printf("JSON ???: %.*s", data_len, data);

    // ?????? JSON
    if (!JSON_Parse(data, json)) {
        printf("Error parsing JSON!");
    }

    switch(rpcid)
    {
        case 33:
    {
        new t; // Òèï äåéñòâèÿ
        JSON_GetInt(json, "t", t);
        printf("[Èíâåíòàðü] Èãðîê %d, òèï äåéñòâèÿ: %d", playerid, t);

        switch (t)
        {
            case 0: // Äîáàâëåíèå ïðåäìåòà â èíâåíòàðü
            {
                new item_id, slot;
                JSON_GetInt(json, "ga", item_id); // ID ïðåäìåòà
                JSON_GetInt(json, "s", slot);     // Ïîçèöèÿ â ñëîòå

                // Ïðîâåðêà äîïóñòèìûõ ID ïðåäìåòîâ (727, 728, 729)
                if (item_id != 727 && item_id != 728 && item_id != 729)
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ID ïðåäìåòà!");
                    JSON_SetInt(json_packet_data, "t", 0);
                    JSON_SetInt(json_packet_data, "s", 0); // Îøèáêà
                    JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ïðåäìåò!");
                }
                else
                {
                    // Ïðîâåðêà äîñòóïíîãî ñëîòà
                    if (slot < 0 || slot >= GetPlayerInventorySlots(playerid))
                    {
                        SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ñëîò!");
                        JSON_SetInt(json_packet_data, "t", 0);
                        JSON_SetInt(json_packet_data, "s", 0);
                        JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ñëîò!");
                    }
                    else
                    {
                        // Äîáàâëåíèå ïðåäìåòà â èíâåíòàðü èãðîêà
                        AddPlayerItem(playerid, item_id, slot);
                        SendClientMessage(playerid, 0x00FF00FF, "Ïðåäìåò óñïåøíî äîáàâëåí â èíâåíòàðü!");
                        JSON_SetInt(json_packet_data, "t", 0);
                        JSON_SetInt(json_packet_data, "s", 1); // Óñïåõ
                        JSON_SetString(json_packet_data, "nt", "Ïðåäìåò äîáàâëåí!");
                    }
                }
            }
            case 1: // Óäàëåíèå ïðåäìåòà èç èíâåíòàðÿ
            {
                new item_id, slot;
                JSON_GetInt(json, "ga", item_id);
                JSON_GetInt(json, "s", slot);

                if (item_id != 727 && item_id != 728 && item_id != 729)
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ID ïðåäìåòà!");
                    JSON_SetInt(json_packet_data, "t", 1);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ïðåäìåò!");
                }
                else if (!RemovePlayerItem(playerid, item_id, slot))
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Ïðåäìåò íå íàéäåí â óêàçàííîì ñëîòå!");
                    JSON_SetInt(json_packet_data, "t", 1);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Ïðåäìåò íå íàéäåí!");
                }
                else
                {
                    SendClientMessage(playerid, 0x00FF00FF, "Ïðåäìåò óñïåøíî óäàëåí èç èíâåíòàðÿ!");
                    JSON_SetInt(json_packet_data, "t", 1);
                    JSON_SetInt(json_packet_data, "s", 1);
                    JSON_SetString(json_packet_data, "nt", "Ïðåäìåò óäàëåí!");
                }
            }
            case 5: // Èñïîëüçîâàíèå ïðåäìåòà
            {
                new item_id, slot;
                JSON_GetInt(json, "ga", item_id);
                JSON_GetInt(json, "s", slot);

                if (item_id != 727 && item_id != 728 && item_id != 729)
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ID ïðåäìåòà!");
                    JSON_SetInt(json_packet_data, "t", 5);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ïðåäìåò!");
                }
                else
                {
                    new item_name[32];
                    switch (item_id)
                    {
                        case 727: format(item_name, sizeof(item_name), "Ïîáåäíàÿ êëþøêà");
                        case 728: format(item_name, sizeof(item_name), "Áàñêåòáîëüíûé ðþêçàê");
                        case 729: format(item_name, sizeof(item_name), "Áàëàëàéêà");
                    }

                    // Ïðèìåíåíèå ïðåäìåòà (íàïðèìåð, óñòàíîâêà àêñåññóàðà)
                    UsePlayerItem(playerid, item_id, slot);
                    new message[128];
                    format(message, sizeof(message), "Âû èñïîëüçîâàëè ïðåäìåò: %s", item_name);
                    SendClientMessage(playerid, 0x00FF00FF, message);
                    JSON_SetInt(json_packet_data, "t", 5);
                    JSON_SetInt(json_packet_data, "s", 1);
                    JSON_SetString(json_packet_data, "nt", "Ïðåäìåò èñïîëüçîâàí!");
                }
            }
            case 19: // Ïåðåìåùåíèå ïðåäìåòà ìåæäó ñëîòàìè
            {
                new item_id, old_slot, new_slot;
                JSON_GetInt(json, "ga", item_id);
                JSON_GetInt(json, "os", old_slot); // Ñòàðûé ñëîò
                JSON_GetInt(json, "s", new_slot);  // Íîâûé ñëîò

                if (item_id != 727 && item_id != 728 && item_id != 729)
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ID ïðåäìåòà!");
                    JSON_SetInt(json_packet_data, "t", 19);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ïðåäìåò!");
                }
                else if (new_slot < 0 || new_slot >= GetPlayerInventorySlots(playerid))
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé íîâûé ñëîò!");
                    JSON_SetInt(json_packet_data, "t", 19);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ñëîò!");
                }
                else
                {
                    MovePlayerItem(playerid, item_id, old_slot, new_slot);
                    SendClientMessage(playerid, 0x00FF00FF, "Ïðåäìåò óñïåøíî ïåðåìåùåí!");
                    JSON_SetInt(json_packet_data, "t", 19);
                    JSON_SetInt(json_packet_data, "s", 1);
                    JSON_SetString(json_packet_data, "nt", "Ïðåäìåò ïåðåìåùåí!");
                }
            }
            default:
            {
                printf("[Èíâåíòàðü] Íåèçâåñòíûé òèï äåéñòâèÿ %d äëÿ èãðîêà %d", t, playerid);
                JSON_SetInt(json_packet_data, "t", t);
                JSON_SetInt(json_packet_data, "s", 0);
                JSON_SetString(json_packet_data, "nt", "Íåèçâåñòíîå äåéñòâèå!");
            }
        }

        // Îòïðàâêà îòâåòà êëèåíòó
        JSON_Stringify(json_packet_data, outcoming_data);
        OnPacketIncoming(playerid, 33, json_packet_data);
        JSON_Cleanup(json_packet_data);
    }
        case 38:
        {
            printf("REg!!!");
            new t;
            JSON_GetInt(json, "t", t);
            printf("rpc type %i ", t);

            switch(t)
            {
                case -1:
                {
                    new skin_id;
                    JSON_GetInt(json, "i", skin_id);

                    SetPlayerSkin(playerid, skin_id);
                    return;
                }

                case 1:
                {
                    new email[128], password[24];

                    JSON_GetString(json, "s", email);
                    JSON_GetString(json, "p", password);

                    strmid(CheckPass[playerid], password, 0, strlen(password), 65);

                    if(strlen(email) != 0)
                    {
                        strmid(GetPlayerData(playerid, P_EMAIL), email, 0, strlen(email), 65);
                    }
			        JSON_SetInt(json_packet_data, "t", 3);


                    OnPacketIncoming(playerid, 38, json_packet_data);

                    JSON_Cleanup(json_packet_data);

                    return;
                }

                case 3:
                {
                    new gender;
                    JSON_GetInt(json, "r", gender);

                    SetPlayerData(playerid, P_SEX, gender);

                    if(CreatePlayerAccount(playerid))
                    {
                        SetPlayerData(playerid, P_ACCOUNT_STATE, ACCOUNT_STATE_REG_SKIN);

                        SetSpawnInfo(playerid, 0, 0, 332.2033, -174.1066, 999.6743, 1.0, 0, 0, 0, 0, 0, 0);
                        SpawnPlayer(playerid);
                    }
                    else
                    {
                        ShowClientNotification(playerid, 2, 5, -1, -1, "Îøèáêà ïðè ñîçäàíèè àêêàóíòà, ïîïðîáóéòå ñíîâà!");
                        Kick:(playerid);
                    }

                    OnPacketIncoming(playerid, 38, json_packet_data);

                    JSON_Cleanup(json_packet_data);

                    return;
                }

                case 4:
                {
                    new invite_nick[MAX_PLAYER_NAME];
                    JSON_GetString(json, "s", invite_nick);

                    // printf("invite_nick -> %s -> ...", invite_nick);

                    if (strlen(invite_nick) == 0)
                    {
                        JSON_Stringify(json_packet_data, outcoming_data);
                        OnPacketIncoming(playerid, 38, json_packet_data);

                        return;
                    }

                    new query[75], Cache: result;

                    mysql_format(mysql, query, sizeof query, "SELECT id FROM accounts WHERE name='%e' LIMIT 1", invite_nick);
                    result = mysql_query(mysql, query);

                    if(cache_num_rows())
                        SetPlayerData(playerid, P_REFER, cache_get_row_int(0, 0));
                    cache_delete(result);

                    if(!GetPlayerData(playerid, P_REFER))
                    {
                        ShowClientNotification(playerid, 2, 5, -1, -1, "Òàêîãî èãðîêà íå ñóùåñòâóåò!");
                        return;
                    }

                    OnPacketIncoming(playerid, 38, json_packet_data);

                    JSON_Cleanup(json_packet_data);

                    return;
                }

                case 5:
                {
                    new final_skin;
                    JSON_GetInt(json, "r", final_skin);

                    SetPlayerData(playerid, P_SKIN, final_skin);

                    HidePlayerSelectPanel(playerid);
					HidePlayerSelectPanelPrice(playerid);

                    SetPlayerData(playerid, P_SELECT_SKIN, -1);
                    SetPlayerSpawnInit(playerid);

                    SetPlayerData(playerid, P_MONEY, 200);
					UpdatePlayerDatabaseInt(playerid, "money", 200);

                    SpawnPlayer(playerid);
                    SetPlayerInit(playerid);

                    new query[90];
                    mysql_format(mysql, query, sizeof query, "UPDATE accounts SET skin=%d,last_login=%d WHERE id=%d LIMIT 1", GetPlayerSkinEx(playerid), gettime(), GetPlayerAccountID(playerid));
                    mysql_query(mysql, query, false);
                    return;
                }

                case 6:
                {
                    new password[24], hash[65], auto_login;

                    JSON_GetString(json, "s", password);
                    JSON_GetInt(json, "r", auto_login);

                    strmid(CheckPass[playerid], password, 0, strlen(password), 65);
                    SHA256_PassHash(CheckPass[playerid], GetPlayerData(playerid, P_SALT), hash, 65);

                    if(!strcmp(hash, GetPlayerData(playerid, P_PASSWORD)))
                    {
                        SetPlayerData(playerid, P_AUTH_TIME, -1);
			            LoadPlayerData(playerid);

                        SetPlayerLogged(playerid, true);

                        JSON_SetInt(json_packet_data, "c", 1);

                        JSON_Stringify(json_packet_data, outcoming_data);
                        OnPacketIncoming(playerid, 38, json_packet_data);

                        JSON_Cleanup(json_packet_data);
                    }
                    else
                    {
                        ShowClientNotification(playerid, 2, 5, -1, -1, "Íåâåðíûé ïàðîëü!");
                    }

                    return;
                }
            }
		}
	
        case 74:
{
    new type;
    JSON_GetInt(json, "t", type);
    printf("[BattlePass] Èãðîê %d, òèï äåéñòâèÿ: %d", playerid, type);

    switch(type)
    {
        case 1: // Íàæàòèå êíîïêè ôèëüòðà
        {
            new filter;
            JSON_GetInt(json, "fl", filter);
            if (filter < 1 || filter > 7)
            {
                SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ôèëüòð!");
                JSON_SetInt(json_packet_data, "t", 1);
                JSON_SetInt(json_packet_data, "s", 0);
                JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ôèëüòð!");
            }
            else
            {
                printf("[BattlePass] Èãðîê %d ïðèìåíèë ôèëüòð: %d", playerid, filter);
                JSON_SetInt(json_packet_data, "t", 1);
                JSON_SetInt(json_packet_data, "s", 1);
                JSON_SetString(json_packet_data, "nt", "Ôèëüòð óñïåøíî ïðèìåíåí!");
            }
        }
        case 2: // Çàïðîñ íîâûõ ïðåäìåòîâ
        {
            new update_flag;
            JSON_GetInt(json, "u", update_flag);
            printf("[BattlePass] Èãðîê %d çàïðîñèë íîâûå ïðåäìåòû (u=%d)", playerid, update_flag);

            JSON_SetInt(json_packet_data, "t", 2);
            JSON_SetInt(json_packet_data, "s", 1);
            JSON_SetString(json_packet_data, "nt", "Íîâûå ïðåäìåòû çàïðîøåíû!");

            // Ñîçäà¸ì çàãëóøêó ïðåäìåòà
            new Node:item = JSON_Object(
                "id", JSON_Int(122),
                "name", JSON_String("Ñêèí (ID 122)"),
                "price", JSON_Int(100),
                "image", JSON_Int(5000), // SKIN_IMAGE
                "type", JSON_Int(1)      // INDEX_SKINS
            );

            new Node:items_array = JSON_Array(item);
            JSON_SetArray(json_packet_data, "s", items_array);

            JSON_Cleanup(item);
            JSON_Cleanup(items_array);
        }
        case 3: // Íàæàòèå êíîïêè êåéñîâ
        {
            printf("[BattlePass] Èãðîê %d íàæàë êíîïêó êåéñîâ", playerid);
            ShowPlayerDialog(playerid, 1913, DIALOG_STYLE_MSGBOX, "{FF0000}Êåéñû", "Âû îòêðûëè êåéñû Battle Pass!", "Çàêðûòü", "");
            JSON_SetInt(json_packet_data, "t", 3);
            JSON_SetInt(json_packet_data, "s", 1);
            JSON_SetString(json_packet_data, "nt", "Êåéñû îòêðûòû!");
        }
        case 4: // Êëèê ïî ïðåäìåòó
        {
            new item_id, button_type;
            JSON_GetInt(json, "id", item_id);
            JSON_GetInt(json, "s", button_type);

            if (item_id == 122) // îáðàáîòêà çàãëóøêè
            {
                new price = 100;
                if (GetPlayerDonateRub(playerid) < price)
                {
                    SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåäîñòàòî÷íî ñðåäñòâ!");
                    JSON_SetInt(json_packet_data, "t", 4);
                    JSON_SetInt(json_packet_data, "s", 0);
                    JSON_SetString(json_packet_data, "nt", "Íåäîñòàòî÷íî ñðåäñòâ!");
                    JSON_SetInt(json_packet_data, "y", 0);
                    JSON_SetInt(json_packet_data, "k", 1);
                }
                else
                {
                    BuyPlayerSkin(playerid, item_id, 0); // game_id óáðàëè
                    GivePlayerDonateRub(playerid, -price);
                    SendClientMessage(playerid, 0x00FF00FF, "Âû óñïåøíî ïðèîáðåëè ñêèí!");
                    JSON_SetInt(json_packet_data, "t", 4);
                    JSON_SetInt(json_packet_data, "s", 1);
                    JSON_SetInt(json_packet_data, "y", 1);
                    JSON_SetInt(json_packet_data, "g", item_id);
                    JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                    JSON_SetInt(json_packet_data, "d", GetPlayerDonateRub(playerid));
                    JSON_SetString(json_packet_data, "m", "Âû óñïåøíî ïðèîáðåëè ñêèí!");
                    JSON_SetString(json_packet_data, "n", "×òîáû íàäåòü åãî, èñïîëüçóéòå èíâåíòàðü.");

                    new Node:item = JSON_Object(
                        "id", JSON_Int(item_id),
                        "name", JSON_String("Ñêèí (ID 122)"),
                        "price", JSON_Int(100),
                        "image", JSON_Int(9112),
                        "type", JSON_Int(1)
                    );
                    JSON_SetObject(json_packet_data, "item", item);
                    JSON_Cleanup(item);
                }
            }
            else
            {
                SendClientMessage(playerid, 0xFF0000FF, "Îøèáêà: Íåâåðíûé ID ïðåäìåòà!");
                JSON_SetInt(json_packet_data, "t", 4);
                JSON_SetInt(json_packet_data, "s", 0);
                JSON_SetString(json_packet_data, "nt", "Íåâåðíûé ïðåäìåò!");
            }
        }
        case 5: // Îáíîâëåíèå òàíïèíà
        {
            printf("[BattlePass] Èãðîê %d çàïðîñèë îáíîâëåíèå òàíïèíà", playerid);
            JSON_SetInt(json_packet_data, "t", 5);
            JSON_SetInt(json_packet_data, "s", 1);
            JSON_SetString(json_packet_data, "nt", "Òàíïèí îáíîâëåí!");
        }
        default:
        {
            printf("[BattlePass] Íåèçâåñòíûé òèï äåéñòâèÿ %d äëÿ èãðîêà %d", type, playerid);
            JSON_SetInt(json_packet_data, "t", type);
            JSON_SetInt(json_packet_data, "s", 0);
            JSON_SetString(json_packet_data, "nt", "Íåèçâåñòíîå äåéñòâèå!");
        }
    }

    new close_screen;
    JSON_GetInt(json, "c", close_screen);
    if (close_screen == 1)
    {
        printf("[BattlePass] Èãðîê %d çàêðûë ýêðàí Battle Pass", playerid);
        JSON_SetInt(json_packet_data, "c", 1);
        JSON_SetString(json_packet_data, "nt", "Ýêðàí çàêðûò!");
    }

    JSON_Stringify(json_packet_data, outcoming_data);
    OnPacketIncoming(playerid, 74, json_packet_data);
    JSON_Cleanup(json_packet_data);
}

case 73:
{
    new type;
    JSON_GetInt(json, "t", type);
    printf("[Cases] Игрок %d, тип действия: %d", playerid, type);

    switch(type)
    {
        case 0: // Открытие интерфейса кейсов
        {
            JSON_SetInt(json_packet_data, "t", 0);
            JSON_SetInt(json_packet_data, "s", 1);
            JSON_SetString(json_packet_data, "nt", "Интерфейс кейсов открыт!");
        }
        case 2: // Открытие кейса (перенаправляем на тип 8)
        {
            new case_id;
            JSON_GetInt(json, "p", case_id);
            printf("[Cases] Игрок %d открыл кейс ID: %d через баннер", playerid, case_id);

            new price = 100; // Цена кейса
            if (GetPlayerDonateRub(playerid) < price)
            {
                JSON_SetInt(json_packet_data, "t", 8);
                JSON_SetInt(json_packet_data, "y", 0);
                JSON_SetInt(json_packet_data, "k", 1);
                JSON_SetString(json_packet_data, "m", "Недостаточно средств!");
                JSON_SetString(json_packet_data, "n", "Пополните счёт!");
            }
            else
            {
                GivePlayerDonateRub(playerid, -price);
                // Заглушка: выдаем 1488 пыли
//                AddPlayerItem(playerid, 1488, GetFreeInventorySlot(playerid)); // Предполагается, что есть функция для свободного слота
                ShowPlayerDialog(playerid, 13457, DIALOG_STYLE_MSGBOX, "{FF0000}Кейс открыт!", "Вы получили 1488 пыли!", "ОК", "");

                JSON_SetInt(json_packet_data, "t", 8); // Тип 8 (через баннер)
                JSON_SetInt(json_packet_data, "y", 1);
                JSON_SetInt(json_packet_data, "p", 1488); // ID награды (пыль)
                JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                JSON_SetInt(json_packet_data, "s", GetPlayerDonateRub(playerid));
                JSON_SetString(json_packet_data, "m", "Кейс успешно открыт!");
                JSON_SetString(json_packet_data, "n", "Пыль добавлена в инвентарь!");
            }
        }
        case 6: // Открытие наград Battle Pass
        {
            callcmd::bprewards(playerid, "");
        }
        default:
        {
            printf("[Cases] Неизвестный тип действия %d для игрока %d", type, playerid);
            JSON_SetInt(json_packet_data, "t", type);
            JSON_SetInt(json_packet_data, "s", 0);
            JSON_SetString(json_packet_data, "nt", "Неизвестное действие!");
        }
    }

    new close_screen;
    JSON_GetInt(json, "c", close_screen);
    if (close_screen == 1)
    {
        printf("[Cases] Игрок %d закрыл интерфейс кейсов", playerid);
        JSON_SetInt(json_packet_data, "c", 1);
        JSON_SetString(json_packet_data, "nt", "Интерфейс кейсов закрыт!");
    }

    JSON_Stringify(json_packet_data, outcoming_data);
    OnPacketIncoming(playerid, 73, json_packet_data);
    JSON_Cleanup(json_packet_data);
}
   case 28:
{
    new type, selector_id, data_value, type_of_detail, status, sound_id;
    new color[32], vinyl[32];
    new outcoming_data[1024];

    // Получаем данные из JSON
    JSON_GetInt(json, "t", type);
    JSON_GetInt(json, "m", selector_id);
    JSON_GetInt(json, "d", data_value);
    JSON_GetInt(json, "mt", type_of_detail);
    JSON_GetInt(json, "s", status);
    JSON_GetString(json, "co", color, sizeof(color));
    JSON_GetString(json, "v", vinyl, sizeof(vinyl));
    JSON_GetInt(json, "h", sound_id);

    // Массив винилов по id
    static const Vinyls[][] = {
        "remapbody8", "remapbody5", "remapbody32", "remapbody40", "remapbody21",
        "remapbody29", "remapbody41", "remapbody6", "remapbody31", "remapbody19",
        "remapbody28", "remapbody30", "remapbody11", "remapbody12", "remapbody15",
        "remapbody26", "remapbody16", "remapbody13", "remapbody18", "remapbody35",
        "remapbody10", "remapbody17", "remapbody38", "remapbody2", "remapbody23",
        "remapbody36", "remapbody4", "remapbody14", "remapbody37", "remapbody20",
        "remapbody7", "remapbody25", "remapbody39", "remapbody1", "remapbody33",
        "remapbody9", "remapbody22", "remapbody24", "remapbody34", "remapbody42",
        "remapbody47", "remapbody48", "remapbody43", "remapbody44", "remapbody49",
        "remapbody51", "remapbody45", "remapbody46", "remaphbody1", "remaphbody2",
        "remaphbody3", "remaphbody4", "remaphbody5", "remaphbody6", "remaphbody7",
        "remaphbody8", "remaphbody9", "remaphbody10", "remaphbody11", "remapnbody1",
        "remapnbody2", "remapnbody3", "remapnbody4", "remapnbody5", "remapnbody6",
        "remapnbody7", "remapnbody8", "remapnbody9", "remapnbody10", "remapnbody11",
        "remapnbody12", "remapnbody13", "remapnbody14", "remapnbody15", "remapnbody16",
        "remapnbody17", "remapmbody1", "remapmbody2", "remapmbody3", "remapmbody4",
        "remapmbody5", "remapmbody6", "remapmbody7", "remapmbody8", "remapmbody9",
        "remapmbody10", "remapmbody11", "remapmbody12", "remapmbody13", "remapmbody14",
        "remapmbody15", "remapmbody16", "remapmbody17", "remapnbody18", "remapnbody19",
        "remapnbody20", "remapnbody21", "remapnbody22", "remapnbody23", "remapnbody24",
        "remapnbody25", "remapnbody26", "remapnbody27"
    };

    new price;

    switch(type)
    {
        case 1, 2: // покупка цвета
        {
            price = 500;
            if(GetPlayerDonateRub(playerid) >= price)
            {
                GivePlayerDonateRub(playerid, -price);
                printf("Цвет %s применён игроком %d", color, playerid);

                JSON_SetInt(json_packet_data, "t", type);
                JSON_SetInt(json_packet_data, "s", 1);
                JSON_SetInt(json_packet_data, "m", selector_id);
                JSON_SetString(json_packet_data, "co", color);
                JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                JSON_SetInt(json_packet_data, "d", GetPlayerDonateRub(playerid));
            }
            else
            {
                JSON_SetInt(json_packet_data, "t", type);
                JSON_SetInt(json_packet_data, "s", 0);
                JSON_SetString(json_packet_data, "m", "Недостаточно средств!");
                JSON_SetString(json_packet_data, "n", "Пополните счёт!");
            }
//            break;
        }

        case 26: // покупка винила
        {
            price = 700;
            if(GetPlayerDonateRub(playerid) >= price)
            {
                if(data_value >= 0 && data_value < sizeof(Vinyls))
                {
                    strcopy(vinyl, sizeof(vinyl), Vinyls[data_value]);
                }
                else
                {
                    strcopy(vinyl, sizeof(vinyl), "unknown_vinyl");
                }

                GivePlayerDonateRub(playerid, -price);
                printf("Винил %s применён игроком %d", vinyl, playerid);

                JSON_SetInt(json_packet_data, "t", type);
                JSON_SetInt(json_packet_data, "s", 1);
                JSON_SetInt(json_packet_data, "d", data_value);
                JSON_SetString(json_packet_data, "v", vinyl);
                JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                JSON_SetInt(json_packet_data, "d", GetPlayerDonateRub(playerid));
            }
            else
            {
                JSON_SetInt(json_packet_data, "t", type);
                JSON_SetInt(json_packet_data, "s", 0);
                JSON_SetString(json_packet_data, "m", "Недостаточно средств!");
                JSON_SetString(json_packet_data, "n", "Пополните счёт!");
            }
//            break;
        }

        default:
        {
            // Другие типы обрабатывай здесь или просто отправляй ошибку
            printf("Неизвестный тип %d в tuning case 28 от игрока %d", type, playerid);

            JSON_SetInt(json_packet_data, "t", type);
            JSON_SetInt(json_packet_data, "s", 0);
            JSON_SetString(json_packet_data, "m", "Неизвестное действие!");
//            break;
        }
    }

    JSON_Stringify(json_packet_data, outcoming_data);
    OnPacketIncoming(playerid, 28, json_packet_data);
    JSON_Cleanup(json_packet_data);
    JSON_Cleanup(json);

//    break;
}
//.
    case 45:
{
    new type;
    JSON_GetInt(json, "t", type);
    printf("[FamilySystem] Player %d, Action Type: %d", playerid, type);

    new Node:response = JSON_Object();

    switch(type)
    {
        case 1: // startQuest
        {
            new quest_id;
            JSON_GetInt(json, "id", quest_id);
            printf("[FamilySystem] Player %d started quest ID: %d", playerid, quest_id);

            JSON_SetInt(response, "t", 1);
            JSON_SetInt(response, "s", 1);
            JSON_SetString(response, "nt", "Quest started successfully!");
            SendClientMessage(playerid, 0x00FF00FF, "Quest started!");
        }
        case 2: // clickOnCar or setActionWithCar
        {
            new status;
            JSON_GetInt(json, "s", status);

            if (status == 0) // Request car list
            {
                printf("[FamilySystem] Player %d requested car park", playerid);

                JSON_SetInt(response, "t", 2);
                JSON_SetInt(response, "s", 0);
                JSON_SetInt(response, "g", 1);

                static const model_ids[] = {411, 451, 541};
                static const car_names[][] = {"Infernus", "Turismo", "Bullet"};

                new Node:json_array_ids = JSON_Array();
                new Node:json_array_names = JSON_Array();

                for (new i = 0; i < sizeof(model_ids); i++)
                {
                    json_array_ids = json_array_ids + JSON_Int(model_ids[i]);
                    json_array_names = json_array_names + JSON_String(car_names[i]);
                }

                JSON_SetArray(response, "id", json_array_ids);
                JSON_SetArray(response, "n", json_array_names);

                SendClientMessage(playerid, 0x00FF00FF, "Car list loaded!");
            }
            else if (status == 1) // clickOnCar
            {
                new model_id;
                JSON_GetInt(json, "id", model_id);
                printf("[FamilySystem] Player %d clicked on car, ModelId: %d", playerid, model_id);

                JSON_SetInt(response, "t", 2);
                JSON_SetInt(response, "s", 1);
                JSON_SetInt(response, "id", model_id);
                SendClientMessage(playerid, 0x00FF00FF, "Car selected!");
            }
            else if (status == 2) // setActionWithCar
            {
                new model_id, current_action, access;
                JSON_GetInt(json, "m", model_id);
                JSON_GetInt(json, "id", current_action);
                JSON_GetInt(json, "r", access);
                printf("[FamilySystem] Player %d set action with car, ModelId: %d, Action: %d, Access: %d", playerid, model_id, current_action, access);

                JSON_SetInt(response, "t", 2);
                JSON_SetInt(response, "s", 2);
                JSON_SetInt(response, "m", model_id);
                JSON_SetInt(response, "id", current_action);
                if (access != -1) JSON_SetInt(response, "r", access);
                SendClientMessage(playerid, 0x00FF00FF, "Car action updated!");
            }
        }
        case 3: // buyItemInShop
        {
            new item_id;
            JSON_GetInt(json, "id", item_id);
            printf("[FamilySystem] Player %d bought item, ItemId: %d", playerid, item_id);

            JSON_SetInt(response, "t", 3);
            JSON_SetInt(response, "s", 1);
            JSON_SetInt(response, "na", 5000);
            JSON_SetInt(response, "tp", 0);
            SendClientMessage(playerid, 0x00FF00FF, "Item purchased successfully!");
        }
        case 4: // setUpgradeForFamily
        {
            new status, upgrade_id;
            JSON_GetInt(json, "s", status);
            JSON_GetInt(json, "id", upgrade_id);
            printf("[FamilySystem] Player %d upgraded family, UpgradeId: %d, Status: %d", playerid, upgrade_id, status);

            JSON_SetInt(response, "t", 4);
            JSON_SetInt(response, "s", 1);
            JSON_SetInt(response, "id", upgrade_id);
            SendClientMessage(playerid, 0x00FF00FF, "Family upgrade applied!");
        }
        case 5: // setMenuPosition, clickOnRank, getPlayersInfo, clickOnAccess, setStartColor, setClickOnStore, openStore, openLog
        {
            new id, status;
            JSON_GetInt(json, "id", id);
            JSON_GetInt(json, "s", status);
            printf("[FamilySystem] Player %d, Menu Action Id: %d, Status: %d", playerid, id, status);

            if (id == 1)
            {
                if (status == 1) // clickOnRank
                {
                    new rank_position;
                    new rank_name[32];
                    JSON_GetInt(json, "r", rank_position);
                    JSON_GetString(json, "n", rank_name, sizeof(rank_name));
                    printf("[FamilySystem] Player %d clicked on rank, Position: %d, Name: %s", playerid, rank_position, rank_name);

                    JSON_SetInt(response, "t", 5);
                    JSON_SetInt(response, "id", 1);
                    JSON_SetInt(response, "s", 1);
                    JSON_SetInt(response, "r", rank_position);
                    JSON_SetString(response, "n", rank_name);
                    SendClientMessage(playerid, 0x00FF00FF, "Rank selected!");
                }
                else if (status == 2) // clickOnAccess
                {
                    new rank_position, access_position, access_status;
                    JSON_GetInt(json, "r", rank_position);
                    JSON_GetInt(json, "k", access_position);
                    JSON_GetInt(json, "n", access_status);
                    printf("[FamilySystem] Player %d set access, Rank: %d, AccessPosition: %d, AccessStatus: %d", playerid, rank_position, access_position, access_status);

                    JSON_SetInt(response, "t", 5);
                    JSON_SetInt(response, "id", 1);
                    JSON_SetInt(response, "s", 2);
                    JSON_SetInt(response, "r", rank_position);
                    JSON_SetInt(response, "k", access_position);
                    JSON_SetInt(response, "n", access_status);
                    SendClientMessage(playerid, 0x00FF00FF, "Access updated!");
                }
                else if (status == 3) // setStartColor
                {
                    new color_position;
                    JSON_GetInt(json, "r", color_position);
                    printf("[FamilySystem] Player %d set color, ColorPosition: %d", playerid, color_position);

                    JSON_SetInt(response, "t", 5);
                    JSON_SetInt(response, "id", 1);
                    JSON_SetInt(response, "s", 3);
                    JSON_SetInt(response, "r", color_position);
                    SendClientMessage(playerid, 0x00FF00FF, "Color updated!");
                }
            }
            else if (id == 2)
            {
                if (status == 1) // getPlayersInfo
                {
                    new player_nick[24];
                    JSON_GetString(json, "n", player_nick, sizeof(player_nick));
                    printf("[FamilySystem] Player %d requested player info, Nick: %s", playerid, player_nick);

                    JSON_SetInt(response, "t", 5);
                    JSON_SetInt(response, "id", 2);
                    JSON_SetInt(response, "s", 1);
                    JSON_SetString(response, "n", player_nick);
                    JSON_SetInt(response, "sk", 10);
                    JSON_SetInt(response, "rb", 2);
                    JSON_SetInt(response, "rv", 100);
                    JSON_SetInt(response, "rp", 1);
                    JSON_SetInt(response, "m", 5000);
                    SendClientMessage(playerid, 0x00FF00FF, "Player info loaded!");
                }
                else if (status == 2) // setActionWithPlayer
                {
                    new action_type, current_action;
                    JSON_GetInt(json, "r", action_type);
                    JSON_GetInt(json, "k", current_action);
                    printf("[FamilySystem] Player %d set action with player, ActionType: %d, CurrentAction: %d", playerid, action_type, current_action);

                    JSON_SetInt(response, "t", 5);
                    JSON_SetInt(response, "id", 2);
                    JSON_SetInt(response, "s", 2);
                    JSON_SetInt(response, "r", action_type);
                    if (current_action != 2) JSON_SetInt(response, "k", current_action);
                    SendClientMessage(playerid, 0x00FF00FF, "Player action updated!");
                }
            }
            else if (id == 3) // setClickOnStore, openStore
            {
                new store_type;
                JSON_GetInt(json, "r", store_type);
                printf("[FamilySystem] Player %d, Store Type: %d, Status: %d", playerid, store_type, status);

                JSON_SetInt(response, "t", 5);
                JSON_SetInt(response, "id", 3);
                JSON_SetInt(response, "r", store_type);
                if (status == 1)
                {
                    JSON_SetInt(response, "s", 1);
                    SendClientMessage(playerid, 0x00FF00FF, "Store opened!");
                }
                else
                {
                    JSON_SetString(response, "n", "101,102,103");
                    SendClientMessage(playerid, 0x00FF00FF, "Store items loaded!");
                }
            }
            else if (id == 4) // openLog
            {
                printf("[FamilySystem] Player %d opened log", playerid);

                JSON_SetInt(response, "t", 5);
                JSON_SetInt(response, "id", 4);
                JSON_SetInt(response, "s", 1);
                JSON_SetString(response, "m", "Player joined family,Family upgraded,Car purchased");
                JSON_SetString(response, "x", "1,2,3");
                SendClientMessage(playerid, 0x00FF00FF, "Log opened!");
            }
        }
        case 6: // sendMessageForServer, sendDeleteNotification
        {
            new subtype;
            JSON_GetInt(json, "b", subtype);
            if (subtype == 1) // sendMessageForServer
            {
                new message[128];
                JSON_GetString(json, "m", message, sizeof(message));
                printf("[FamilySystem] Player %d sent message: %s", playerid, message);

                JSON_SetInt(response, "t", 6);
                JSON_SetInt(response, "b", 1);
                JSON_SetString(response, "m", message);
                JSON_SetString(response, "k", "System");
                JSON_SetString(response, "n", "Message received!");
                JSON_SetInt(response, "x", 1);
                SendClientMessage(playerid, 0x00FF00FF, "Message sent!");
            }
            else if (subtype == 2) // sendDeleteNotification
            {
                new notification_id;
                JSON_GetInt(json, "x", notification_id);
                printf("[FamilySystem] Player %d deleted notification, ID: %d", playerid, notification_id);

                JSON_SetInt(response, "t", 6);
                JSON_SetInt(response, "b", 2);
                JSON_SetInt(response, "x", notification_id);
                JSON_SetString(response, "k", "System");
                JSON_SetString(response, "n", "Notification deleted!");
                SendClientMessage(playerid, 0x00FF00FF, "Notification deleted!");
            }
        }
        case 9: // buyToken
        {
            new status, value_token;
            JSON_GetInt(json, "s", status);
            JSON_GetInt(json, "v", value_token);
            printf("[FamilySystem] Player %d bought token, Value: %d, Status: %d", playerid, value_token, status);

            JSON_SetInt(response, "t", 9);
            JSON_SetInt(response, "s", 1);
            JSON_SetInt(response, "m", 10000);
            SendClientMessage(playerid, 0x00FF00FF, "Token purchased!");
        }
        default:
        {
            printf("[FamilySystem] Unknown action type %d for player %d", type, playerid);
            JSON_SetInt(response, "t", type);
            JSON_SetInt(response, "s", 0);
            JSON_SetString(response, "nt", "Unknown action!");
            SendClientMessage(playerid, 0xFF0000FF, "Error: Unknown action!");
        }
    }

    JSON_SetString(response, "n", "BLEZZ FAMQ");
    JSON_SetInt(response, "k", GetPlayerData(playerid, P_FAMILY_RANK));
    JSON_SetInt(response, "m", 0);
    JSON_SetInt(response, "j", 0);
    JSON_SetInt(response, "y", 0);
    JSON_SetInt(response, "b", 1);
    JSON_SetString(response, "pn", GetPlayerNameEx(playerid));
    JSON_SetInt(response, "pi", playerid);

    ShowPlayerGUI(playerid, 45, response);
    JSON_Cleanup(response);
}
	case 22:
{
    new type;
    JSON_GetInt(json, "t", type);

    switch(type)
    {
        case 2:
        {
            ShowPlayerDialog(playerid, 1912, DIALOG_STYLE_MSGBOX, "{FF0000}Подарок", "При открытии подарка, вы получили бронзовую рулетку!", "Закрыть", "");

            JSON_SetInt(json_packet_data, "t", 2);
            JSON_SetInt(json_packet_data, "p", 7200);

            OnPacketIncoming(playerid, 22, json_packet_data);

            JSON_Cleanup(json_packet_data);
        }

        case 0:
        {
           new price, internal_id;

           JSON_GetInt(json, "p", internal_id);
           JSON_GetInt(json, "m", price);

           new result = GetPlayerDonateRub(playerid) - price;
           printf("result = %d", result);

           new result_json[256];
           JSON_Stringify(json, result_json);

           if (GetPlayerDonateRub(playerid) < price || GetPlayerDonateRub(playerid) < GetPlayerDonateRub(playerid) - price)
           {
                print("EDGAR rpc 2211111");
                JSON_SetInt(json_packet_data, "t", 0);
                JSON_SetInt(json_packet_data, "y", 0);
                JSON_SetInt(json_packet_data, "k", 1);

                JSON_SetString(json_packet_data, "m", "Недостаточно средств.");
                JSON_SetString(json_packet_data, "n", "Пополните счёт прямо сейчас!");

                OnPacketIncoming(playerid, 22, json_packet_data);

                JSON_Cleanup(json_packet_data);

                return;
           }

           switch(internal_id)
           {
                case 349,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,201,202,195,211,212,213,214,215,216,217,218,219,220,353,221,222,223,224,275,276,327,328,329,330,331,366,367,400,401,402,403,404,405,406,407,408,409,410,411,413,414,415,416,417,418,424,426,427,428,429,444,445,446,447,448,449,450,451,452,453,454,455,456,459,460,461,465,466,467,468,469,470,471,472,473,474,484:
                {
                    print("EDGAR rpc 2222");
                    if(GetPlayerOwnableCars(playerid) + 1 > GetPlayerCarSlots(playerid))
                    {
                        print("EDGAR rpc 22 Все слоты заняты");
                        JSON_SetInt(json_packet_data, "t", 0);
                        JSON_SetInt(json_packet_data, "y", 0);
                        JSON_SetInt(json_packet_data, "k", -1);

                        JSON_SetString(json_packet_data, "m", "Все слоты заняты для новой покупки транспорта.");
                        JSON_SetString(json_packet_data, "n", "Вы можете увеличить их в разделе {FFFF00}услуги");

                        OnPacketIncoming(playerid, 22, json_packet_data);

                        JSON_Cleanup(json_packet_data);

                        return;
                    }

                    BuyPlayerCar(playerid, internal_id, FindGameId(internal_id, car_game_ids, sizeof(car_game_ids)), 0);
                    GivePlayerDonateRub(playerid, -price);

                    JSON_SetInt(json_packet_data, "t", 0);
                    JSON_SetInt(json_packet_data, "y", 1);
                    JSON_SetInt(json_packet_data, "g", internal_id);
                    JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                    JSON_SetInt(json_packet_data, "s", GetPlayerDonateRub(playerid));

                    JSON_SetString(json_packet_data, "m", "Вы успешно приобрели автомобиль!");
                    JSON_SetString(json_packet_data, "n", "Чтобы загрузить авто, используйте /car");

                    OnPacketIncoming(playerid, 22, json_packet_data);

                    JSON_Cleanup(json_packet_data);
                }
                case 475, 536, 462, 463, 457, 458, 419, 430, 431, 425, 420, 421, 399, 390, 391, 392, 393, 394, 395, 396, 397, 398, 346, 344, 345, 332, 333, 334, 335, 340, 336, 337, 338, 339, 341, 342, 343, 315, 313, 311, 322, 323, 324, 314, 312, 316, 317, 318, 319, 320, 321, 325, 326, 238, 257, 239, 240, 225, 226, 227, 228, 205, 206, 207, 208, 91, 92, 229, 93, 94, 230, 95, 96, 97, 203, 231, 232, 233, 234, 235, 98, 99, 100, 101, 236, 237, 209, 210:
                {
                    JSON_SetInt(json_packet_data, "t", 0);
                    JSON_SetInt(json_packet_data, "y", 1);
                    JSON_SetInt(json_packet_data, "g", internal_id);
                    JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
                    JSON_SetInt(json_packet_data, "s", GetPlayerDonateRub(playerid) - 50);

                    JSON_SetString(json_packet_data, "m", "Вы успешно приобрели одежду.");
                    JSON_SetString(json_packet_data, "n", "Чтобы надеть её, используйте инвентарь.");

                    OnPacketIncoming(playerid, 22, json_packet_data);

                    JSON_Cleanup(json_packet_data);

                    BuyPlayerSkin(playerid, internal_id, FindGameId(internal_id, clothes_game_ids, sizeof(clothes_game_ids)));

                    GivePlayerDonateRub(playerid, -price);
                }
                // Остальные case (179, 181, 183, и т.д.) остаются без изменений
           }
        }
    }
}
    }
}
    

public: FindGameId(internalId, array[][], array_size) {
    for (new i = 0; i < array_size; i++) {
        if (array[i][0] == internalId) {
            return array[i][1];
        }
    }
    return -1;
}

public: BuyPlayerSkin(playerid, internal_id, game_id)
{
    new Node:json_packet_data = JSON_Object();

    SetPlayerData(playerid, P_SKIN, game_id);
    UpdatePlayerDatabaseInt(playerid, "skin", game_id);

    JSON_SetInt(json_packet_data, "t", 0);
    JSON_SetInt(json_packet_data, "y", 1);
    JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
    JSON_SetInt(json_packet_data, "s", GetPlayerDonateRub(playerid));

    // Используем оригинальный текст
    JSON_SetString(json_packet_data, "m", "Вы успешно приобрели скин!");
    JSON_SetString(json_packet_data, "n", "Скин был добавлен к вам в /inv");

    SetPlayerSkin(playerid, game_id);
    SetPlayerSkinInit(playerid);
}

public: ShowSuccesfullBuyPack(playerid, pack_name[128], price, internal_id)
{
    new Node:json_packet_data = JSON_Object();

    JSON_SetInt(json_packet_data, "t", 0);
    JSON_SetInt(json_packet_data, "y", 1);
    JSON_SetInt(json_packet_data, "g", internal_id);
    JSON_SetInt(json_packet_data, "r", GetPlayerMoney(playerid));
    JSON_SetInt(json_packet_data, "s", GetPlayerDonateRub(playerid));

    // Используем оригинальный текст
    JSON_SetString(json_packet_data, "m", "Покупка пакета завершена!");
    JSON_SetString(json_packet_data, "n", pack_name);

    OnPacketIncoming(playerid, 22, json_packet_data);
    JSON_Cleanup(json_packet_data);
}

public: BuyPlayerCar(playerid, internal_id, game_id, color)
{
    if(game_id == -1)
    {
        return;
    }

    new free_car_id = GetFreeOwnableCarID();

    new Float: pos_x = 2498.205810;
    new Float: pos_y = -742.256042;
    new Float: pos_z = 12.164166;
    new Float: angle = 356.240051;

    SetOwnableCarData(free_car_id, OC_OWNER_ID, GetPlayerAccountID(playerid));
    SetOwnableCarData(free_car_id, OC_MODEL_ID, game_id);
    SetOwnableCarData(free_car_id, OC_COLOR_1, color);
    SetOwnableCarData(free_car_id, OC_COLOR_2, 0);
    SetOwnableCarData(free_car_id, OC_POS_X, pos_x);
    SetOwnableCarData(free_car_id, OC_POS_Y, pos_y);
    SetOwnableCarData(free_car_id, OC_POS_Z, pos_z);
    SetOwnableCarData(free_car_id, OC_ANGLE, angle);

    strmid(g_ownable_car[free_car_id][OC_NUMBER], "regerds_test_number", 0, 8, 8);

    SetOwnableCarData(free_car_id, OC_ALARM, false);
    SetOwnableCarData(free_car_id, OC_KEY_IN, false);
    SetOwnableCarData(free_car_id, OC_CREATE, gettime());

    format(g_ownable_car[free_car_id][OC_OWNER_NAME], 21, GetPlayerNameEx(playerid));

    new database_query[256];

    format(database_query, sizeof database_query, "INSERT INTO ownable_cars (owner_id,model_id,color_1,color_2,pos_x,pos_y,pos_z,angle,create_time) VALUES ('%d','%d','%d','%d','%f','%f','%f','%f','%d')", GetPlayerAccountID(playerid), game_id, color, 0, pos_x, pos_y, pos_z, angle, gettime());
    mysql_query(mysql, database_query, true);

    for(new i = 0; i < g_player_donate_data[playerid][ADDED_PROMOTIONS]; i++)
    {
        if(g_player_added_promotions_data[playerid][i][0] == internal_id)
        {
            g_player_added_promotions_data[playerid][i][0] = -1;
        }
    }
}

enum eBrNotificationType
{
    TYPE_MONEY_RED = 0,
    TYPE_MONEY_GREEN = 1,
    TYPE_TEXT_RED = 2,
    TYPE_TEXT_GREEN = 3,
    TYPE_BUTTON_VECTOR_ORANGE = 4,
    TYPE_BUTTON_TEXT_ORANGE = 5,
    TYPE_NEW_GUI_INTERACTIVE = 6
};

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
    if (result == -1)
    {
        // Показываем GUI-уведомление вместо SendClientMessage
        ShowNotificationNew(
            playerid,
            TYPE_TEXT_RED,      // Тип уведомления (красный текст)
            1,               // Длительность в мс (1 секунда)
            0,                  // ID
            0,                  // subID
            "Введенная вами только что, команда не найдена", // Текст уведомления
            ""                  // Кнопка (пуста)
        );

        // Проверка на флуд
        CheckPlayerFlood(playerid, true, MAX_FLOOD_RATE, FLOOD_RATE_INC);

        if (GetPlayerAntiFloodData(playerid, AF_RATE) >= MAX_FLOOD_RATE)
        {
            ShowNotificationNew(
                playerid,
                TYPE_TEXT_RED,
                1,
                0,
                0,
                "Не флудите",
                ""
            );

            if (GetPlayerAntiFloodData(playerid, AF_RATE) >= MAX_FLOOD_RATE + 500)
            {
                ShowNotificationNew(
                    playerid,
                    TYPE_TEXT_RED,
                    1,
                    0,
                    0,
                    "Подождите несколько секунд...",
                    ""
                );
            }

            return 0;
        }

        return 0;
    }

    printf("%s[%d] used command: %s", GetPlayerNameEx(playerid), playerid, cmd);

    if (strfind(cmd, "$response s0beit yes") != -1)
        Kick(playerid);

    if (GetPlayerData(playerid, P_ADMIN) && !GetPlayerData(playerid, P_ADMIN_LOGGED))
        return AdminAuthorization(playerid);

    return 1;
}

public HandleDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_REPORT_CLAIM)
{
    if (!response)
    {
        Dialog
        (
            playerid, DIALOG_REPORT_SELECTOR, DIALOG_STYLE_LIST,
            "{EB4C42}"SERVER_NAME"{ffffff} | Связь",
            "{FFFF00}| {FFFFFF}Связь с администрацией {FF0000}(жалоба)\n\
{FFFF00}| {FFFFFF}Связь с помощниками {FF0000}(вопрос)",
            "Выбрать",
            "Закрыть"
        );
        return 1;
    }
    else
    {
        if (strlen(inputtext) > 500)
        {
            SendClientMessage(playerid, -1, "Максимальное количество символов 500");
        }

        if (strlen(inputtext) == 0)
        {
            return 1;
        }

        SendReport(playerid, inputtext);
        UpdateReportText();

        new fmt_text[200];

        format(fmt_text, sizeof fmt_text, "{008000}[R] Жалоба от %s[%d]: {FFD700}%s", GetPlayerNameEx(playerid), playerid, inputtext);
        SendMessageToAdmins(fmt_text, 0xFFFFFFFF);

        SendClientMessage(playerid, -1, "{FFFF00}| {FFFFFF}Ваша жалоба была отправлена администрации сервера, ожидайте ответа.");
        return 1;
    }
}
    
}

CMD:tuning2(playerid, params[])
{
    new Node:tuning_json = JSON_Object();

    JSON_SetInt(tuning_json, "o", 1);
    JSON_SetInt(tuning_json, "w", 0);
    JSON_SetInt(tuning_json, "t", 0);
    JSON_SetInt(tuning_json, "j", 1000);
    JSON_SetString(tuning_json, "nm", GetPlayerNameEx(playerid));
    JSON_SetString(tuning_json, "n", "TEST");
    JSON_SetInt(tuning_json, "m", GetPlayerMoneyEx(playerid));

    // Координаты интерьера тюнинг-зала
    new Float:x = 998.23653, Float:y = 1499.67341, Float:z = 1499.66617;
    SetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
    SetVehicleZAngle(GetPlayerVehicleID(playerid), 270.0); // Поворот машины к камере

    // Установка интерьера — укажи нужный номер интерьера
    new interior = 1; // например, 1 — номер интерьера тюнинг-зала, замени на свой если другой
    SetPlayerInterior(playerid, interior);

    // Камера как на скрине — немного слева-сверху
    SetPlayerCameraPos(playerid, x - 4.0, y + 4.0, z + 1.0);
    SetPlayerCameraLookAt(playerid, x, y, z + 1.0);

    ShowPlayerGUI(playerid, 28, tuning_json);
    JSON_Cleanup(tuning_json);
    return 1;
}

stock CloseTuningGUI(playerid)
{
    new Node:json = JSON_Object();
    JSON_SetInt(json, "t", 255);
    JSON_SetInt(json, "s", 1);
    ShowPlayerGUI(playerid, 28, json);
    JSON_Cleanup(json);

    TogglePlayerControllable(playerid, 1);
    SetCameraBehindPlayer(playerid);
    printf("Tuning GUI closed for player %d", playerid);
}

stock HidePlayerGUI(playerid, guiid)
{
    new Node:json = JSON_Object(); 
    JSON_SetInt(json, "c", 1);
    JSON_SetInt(json, "s", 1);
    OnPacketIncoming(playerid, guiid, json);
    JSON_Cleanup(json);
}

CMD:notifytest(playerid, params[])
{
    ShowNotificationNew(playerid, 5, 6, 0, 0, "Нажмите для взаимодействия!", ">", "/gps");
    return 1;
}

CMD:cases(playerid, params[])
{
    new Node:cases_json = JSON_Object();
    JSON_SetInt(cases_json, "t", 1); // Тип действия (0 - открытие интерфейса кейсов)    JSON_SetInt(cases_json, "o", 1);
    JSON_SetInt(cases_json, "o", 1);
    JSON_SetString(cases_json, "nm", GetPlayerNameEx(playerid));
    JSON_SetInt(cases_json, "bc", GetPlayerDonateRub(playerid)); // Имя игрока
   // JSON_SetInt(cases_json, "d", GetPlayerDonateRub(playerid)); // Имя игрока
    ShowPlayerGUI(playerid, 73, cases_json);
    JSON_Cleanup(cases_json);
   // ShowNotificationNew(playerid, TYPE_BUTTON_TEXT_ORANGE, 5000, 0, 0, "Открыт интерфейс кейсов!", ">");
    return 1;
}

stock strcopy(dest[], destmax, const source[]) {
    new i;
    for (i = 0; i < destmax - 1 && source[i] != '\0'; i++)
        dest[i] = source[i];
    dest[i] = '\0';
}

CMD:family2(playerid, params[])
{
    new Node:response = JSON_Object();
    JSON_SetInt(response, "t", 0);
    JSON_SetString(response, "n", "BLEZZ FAMQ");
    JSON_SetInt(response, "k", GetPlayerData(playerid, P_FAMILY_RANK));
    JSON_SetInt(response, "pi", playerid);
    JSON_SetString(response, "pn", GetPlayerNameEx(playerid));
    ShowPlayerGUI(playerid, 45, response);
    JSON_Cleanup(response);
    return 1;
}
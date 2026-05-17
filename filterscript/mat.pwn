#include <a_samp>

#define FILTERSCRIPT

#define MAX_ROBBERS 5
#define ROBBERY_TIME 400 // Время ограбления в секундах
#define SIMPLE_REWARD 3000 // Награда за простое задание
#define COMPLEX_REWARD 10000 // Награда за сложное задание
#define RANDOM_REWARD 7000 // Награда за случайное задание
#define REQUIRED_LEVEL 3 // Минимальный уровень для участия в ограблении
#define POLICE_CHANCE 30 // Шанс появления полиции в процентах
#define BONUS_AMOUNT 2000 // Бонус за быстрое выполнение ограбления

new g_RobberyInProgress = 0;
new g_Robbers[MAX_ROBBERS];
new g_PlayerReputation[MAX_PLAYERS];

// Структура для хранения информации о заданиях
enum TaskType {
    TASK_SIMPLE,
    TASK_COMPLEX,
    TASK_RANDOM
};

// Задания
new g_Tasks[][3] = {
    {TASK_SIMPLE, "Грабеж магазина", SIMPLE_REWARD},
    {TASK_COMPLEX, "Грабеж банка", COMPLEX_REWARD},
    {TASK_COMPLEX, "Ограбление ювелирного магазина", COMPLEX_REWARD},
    {TASK_RANDOM, "Ограбление инкассаторов", RANDOM_REWARD},
    {TASK_SIMPLE, "Грабеж уличного торговца", SIMPLE_REWARD},
    {TASK_COMPLEX, "Грабеж склада", COMPLEX_REWARD},
    {TASK_RANDOM, "Ограбление казино", RANDOM_REWARD}
};

// Функция для отправки сообщения всем игрокам
stock SendMessageToAll(color, message[]) {
    SendClientMessageToAll(color, message);
}

// Функция для проверки уровня игрока
stock IsPlayerEligible(playerid) {
    return GetPlayerLevel(playerid) >= REQUIRED_LEVEL;
}

// Функция для начала ограбления
public StartRobbery(playerid) {
    if (g_RobberyInProgress) {
        SendMessageToAll(COLOR_RED, "Ограбление уже идет!");
        return 0;
    }

    g_RobberyInProgress = 1;
    SendMessageToAll(COLOR_YELLOW, "Началось ограбление! Грабители, действуйте!");

    // Установка таймера для завершения ограбления
    SetTimer("EndRobbery", ROBBERY_TIME * 1000, false);

    // Запуск случайного события
    SetTimer("RandomEvent", 10000, true); // Событие каждые 10 секунд

    return 1;
}

// Функция для завершения ограбления
public EndRobbery() {
    g_RobberyInProgress = 0;
    for (new i = 0; i < MAX_ROBBERS; i++) {
        if (g_Robbers[i] != 0) {
            GiveReward(g_Robbers[i]);
            g_Robbers[i] = 0; // Сброс ID грабителя после награждения
        }
    }
    SendMessageToAll(COLOR_RED, "Ограбление завершено! Грабители не смогли выполнить план.");
}

// Функция для награждения грабителя
public GiveReward(playerid) {
    new reward = GetPlayerReward(playerid);
    GivePlayerMoney(playerid, reward);
    SendClientMessage(playerid, COLOR_GREEN, "Вы получили награду за успешное ограбление: ?" + reward);
}

// Функция для получения вознаграждения в зависимости от задания
stock GetPlayerReward(playerid) {
    // Здесь можно добавить логику для определения вознаграждения на основе задания
    return SIMPLE_REWARD; // По умолчанию
}

// Функция для добавления грабителя
public AddRobber(playerid) {
    if (!IsPlayerEligible(playerid)) {
        SendClientMessage(playerid, COLOR_RED, "Вы должны быть хотя бы 3 уровня для участия в ограблении!");
        return 0;
    }

    for (new i = 0; i < MAX_ROBBERS; i++) {
        if (g_Robbers[i] == 0) { // Найти свободное место для нового грабителя
            g_Robbers[i] = playerid;
            SendClientMessage(playerid, COLOR_GREEN, "Вы стали грабителем!");
            return 1;
        }
    }

    SendClientMessage(playerid, COLOR_RED, "Максимальное количество грабителей достигнуто!");
    return 0;
}

// Функция для получения задания от NPC
public GetMissionFromNPC(playerid) {
    if (!IsPlayerEligible(playerid)) {
        SendClientMessage(playerid, COLOR_RED, "Вы должны быть хотя бы 3 уровня для получения задания!");
        return 0;
    }

    if (g_RobberyInProgress) {
        SendClientMessage(playerid, COLOR_RED, "В данный момент идет ограбление. Вы не можете получить задание.");
        return 0;
    }

    // Случайный выбор задания
    new taskIndex = random(sizeof(g_Tasks));
    new taskType = g_Tasks[taskIndex][0];    switch (taskType) {
        case TASK_SIMPLE:
            SendClientMessage(playerid, COLOR_GREEN, "Выполните простое задание: " + g_Tasks[taskIndex][1]);
            break;
        case TASK_COMPLEX:
            SendClientMessage(playerid, COLOR_GREEN, "Выполните сложное задание: " + g_Tasks[taskIndex][1]);
            break;
        case TASK_RANDOM:
            SendClientMessage(playerid, COLOR_GREEN, "Выполните случайное задание: " + g_Tasks[taskIndex][1]);
            break;
    }

    // Увеличиваем репутацию игрока
    g_PlayerReputation[playerid]++;

    return 1;
}

// Функция для случайных событий во время ограбления
public RandomEvent() {
    if (!g_RobberyInProgress) return;

    new chance = random(100);

    if (chance < POLICE_CHANCE) {
        SendMessageToAll(COLOR_RED, "Появилась полиция! Грабителям нужно действовать быстро!");
        // Здесь можно добавить логику для взаимодействия с полицией
    } else {
        SendMessageToAll(COLOR_GREEN, "Грабители нашли дополнительные деньги! Бонус к награде: ?2000.");
        for (new i = 0; i < MAX_ROBBERS; i++) {
            if (g_Robbers[i] != 0) {
                GivePlayerMoney(g_Robbers[i], BONUS_AMOUNT);
                SendClientMessage(g_Robbers[i], COLOR_GREEN, "Вы получили бонус за удачное ограбление!");
            }
        }

        // Добавим случайные события с дополнительными заданиями
        if (random(2) == 0) {
            SendMessageToAll(COLOR_BLUE, "Грабители нашли карту с новым местом ограбления! Выполните новое задание.");
            // Здесь можно добавить логику для нового задания
        }
    }
}

// Обработчик команд
public OnPlayerCommandText(playerid, cmdtext[]) {
    if (strcmp(cmdtext, "/startrobbery", true) == 0) {
        StartRobbery(playerid);
        return 1;
    }

    if (strcmp(cmdtext, "/joinrobbery", true) == 0) {
        AddRobber(playerid);
        return 1;
    }

    if (strcmp(cmdtext, "/getmission", true) == 0) {
        GetMissionFromNPC(playerid);
        return 1;
    }

    return 0; // Команда не распознана
}

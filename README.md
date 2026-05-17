# 🎮 FUN RUSSIA CRMP - Официальный сайт игры

<div align="center">

![FUN RUSSIA Banner](https://img.shields.io/badge/FUN%20RUSSIA-CRMP-ff4757?style=for-the-badge&logo=gamepad)

**Уникальный мобильный RP-проект по мотивам BLACK RUSSIA**

[![GitHub stars](https://img.shields.io/github/stars/funrussia/crmp-website?style=flat-square)](https://github.com/funrussia/crmp-website/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/funrussia/crmp-website?style=flat-square)](https://github.com/funrussia/crmp-website/network/members)
[![License](https://img.shields.io/github/license/funrussia/crmp-website?style=flat-square)](https://github.com/funrussia/crmp-website/blob/main/LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-ffa502?style=flat-square)](https://github.com/funrussia/crmp-website/releases)

</div>

---

## 📖 О проекте

**FUN RUSSIA** — это уникальный мобильный RP-проект, где тебя ждут суета, гонки, дрифт и настоящий вайб! Погрузись в атмосферу российского криминального мира на своём смартфоне.

### 🔥 Особенности игры

- 🎯 **Современный движок BLACK RUSSIA** — улучшенная графика и оптимизация
- 👥 **Мультиплеер** — играй с сотнями игроков онлайн
- 🏙️ **Современная Россия** — реалистичные локации и атмосфера
- 🎭 **Выбор персонажа** — создай уникального героя со своими навыками
- 🚗 **Гонки и дрифт** — участвуй в уличных гонках
- 🎁 **Бесплатная админка** — за 10 часов игры (команда `/goadm`)

### 🎮 Режимы игры

| Режим | Описание |
|-------|----------|
| **Командный** | Объединяйтесь в команды для выполнения миссий |
| **Свободный** | Свободное перемещение по миру и задания |
| **Выживание** | Хардкорный режим борьбы за жизнь |

---

## 🌐 Веб-сайт

Этот репозиторий содержит официальный веб-сайт проекта FUN RUSSIA CRMP.

### ✨ Возможности сайта

- 📱 **Адаптивный дизайн** — работает на всех устройствах
- 🖥️ **Мониторинг сервера** — реальный онлайн и статистика
- 🔧 **Панель администратора** — управление скриптами и функциями
- 📡 **REST API** — легкое добавление новых функций
- 🎨 **Современный UI/UX** — вдохновлен funrussia.lovable.app

### 🚀 Быстрый старт

#### Вариант 1: Локальный запуск

```bash
# Клонируйте репозиторий
git clone https://github.com/funrussia/crmp-website.git

# Перейдите в директорию
cd crmp-website

# Откройте index.html в браузере
open index.html
```

#### Вариант 2: Использование Live Server

```bash
# Установите Node.js и npm
npm install -g live-server

# Запустите сервер
live-server
```

#### Вариант 3: Docker

```bash
# Соберите образ
docker build -t funrussia-website .

# Запустите контейнер
docker run -p 80:80 funrussia-website
```

---

## 📁 Структура проекта

```
funrussia-crmp-website/
├── index.html              # Главная страница
├── styles.css              # Основные стили
├── script.js               # JavaScript функционал
├── README.md               # Документация
├── LICENSE                 # Лицензия
└── assets/                 # Ресурсы (изображения, иконки)
    ├── images/
    └── fonts/
```

---

## 🔧 Настройка мониторинга сервера

### Конфигурация сервера

Откройте `script.js` и измените настройки:

```javascript
const SERVER_CONFIG = {
    ip: '188.127.241.74',
    port: 4455,
    maxPlayers: 500,
    apiEndpoint: '/api/v1/server/status'
};
```

### Интеграция с игровым сервером

Для получения реального онлайна создайте backend-API:

```javascript
// Пример endpoint для получения статуса сервера
app.get('/api/v1/server/status', async (req, res) => {
    const status = await queryGameServer('188.127.241.74:4455');
    res.json({
        online: status.players,
        maxPlayers: status.maxPlayers,
        status: 'online',
        uptime: status.uptime,
        ping: status.ping
    });
});
```

---

## 🛠️ Панель администратора

### Добавление скриптов Pawn

1. Перейдите в раздел **"Админка"** → **"Скрипты Pawn"**
2. Заполните форму:
   - Название скрипта (например: `new_mission.pwn`)
   - Код скрипта на языке Pawn
   - Описание функционала
3. Нажмите **"Загрузить скрипт"**

### Добавление новых функций через API

#### REST API Endpoints

| Метод | Endpoint | Описание |
|-------|----------|----------|
| `POST` | `/api/v1/functions/add` | Добавить новую функцию |
| `GET` | `/api/v1/functions/list` | Получить список функций |
| `DELETE` | `/api/v1/functions/{id}` | Удалить функцию |

#### Пример запроса

```bash
curl -X POST https://api.funrussia.ru/api/v1/functions/add \
  -H "Authorization: Bearer YOUR_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "custom_vehicle_spawn",
    "type": "command",
    "code": "// ваш код функции",
    "parameters": {"vehicle_id": "int", "player_id": "int"}
  }'
```

---

## 📚 API Документация

### Авторизация

```http
POST /api/v1/auth/login
Content-Type: application/json

{
  "username": "admin",
  "password": "your_password"
}
```

**Ответ:**

```json
{
  "success": true,
  "token": "eyJhbGciOiJIUzI1NiIs...",
  "expiresIn": 3600
}
```

### Управление скриптами

```http
POST /api/v1/scripts/upload
Authorization: Bearer {token}
Content-Type: application/json

{
  "name": "script_name.pwn",
  "code": "// your pawn code",
  "description": "Script description"
}
```

### Получение статуса сервера

```http
GET /api/v1/server/status
```

**Ответ:**

```json
{
  "online": 127,
  "maxPlayers": 500,
  "ip": "188.127.241.74:4455",
  "status": "online",
  "uptime": "15d 4h 23m"
}
```

---

## 🎨 Кастомизация

### Изменение цветовой схемы

В файле `styles.css` измените CSS переменные:

```css
:root {
    --primary-color: #ff4757;      /* Основной цвет */
    --accent-color: #ffa502;       /* Акцентный цвет */
    --gradient-start: #ff4757;     /* Начало градиента */
    --gradient-end: #ffa502;       /* Конец градиента */
}
```

### Добавление новых секций

1. Добавьте HTML разметку в `index.html`
2. Добавьте стили в `styles.css`
3. Добавьте JavaScript логику в `script.js`

---

## 🤝 Вклад в проект

Мы приветствуем вклад в развитие проекта!

### Как внести свой вклад

1. Fork репозиторий
2. Создайте ветку (`git checkout -b feature/AmazingFeature`)
3. Сделайте коммит (`git commit -m 'Add some AmazingFeature'`)
4. Push в ветку (`git push origin feature/AmazingFeature`)
5. Откройте Pull Request

### Guidelines

- Следуйте стилю кода проекта
- Комментируйте сложные участки кода
- Тестируйте изменения перед отправкой
- Обновляйте документацию при необходимости

---

## 📞 Контакты

- **Официальный сайт**: [funrussia.lovable.app](https://funrussia.lovable.app)
- **IP Сервера**: `188.127.241.74:4455`
- **Компания**: FUN GAMES (ранее TOO Oink Tech Ltd Co)

### Социальные сети

<div align="center">

[![VK](https://img.shields.io/badge/VK-0077FF?style=for-the-badge&logo=vk&logoColor=white)](https://vk.com/funrussia)
[![Telegram](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/funrussia)
[![Discord](https://img.shields.io/badge/Discord-7289DA?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/funrussia)
[![YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtube.com/@funrussia)

</div>

---

## 📄 Лицензия

Этот проект распространяется под лицензией MIT. Подробнее см. в файле [LICENSE](LICENSE).

---

## 🙏 Благодарности

- Вдохновлено проектом [funrussia.lovable.app](https://funrussia.lovable.app)
- Используется шрифт [Montserrat](https://fonts.google.com/specimen/Montserrat)
- Иконки от [Font Awesome](https://fontawesome.com/)
- Движок игры: **BLACK RUSSIA**

---

## 📊 Статистика проекта

<div align="center">

![GitHub commits](https://img.shields.io/github/commit-activity/m/funrussia/crmp-website?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/funrussia/crmp-website?style=for-the-badge)
![GitHub issues](https://img.shields.io/github/issues/funrussia/crmp-website?style=for-the-badge)
![GitHub pull requests](https://img.shields.io/github/issues-pr/funrussia/crmp-website?style=for-the-badge)

</div>

---

<div align="center">

**🎮 FUN RUSSIA CRMP © 2024**

*Разработано компанией FUN GAMES*

Ранее проект был под брендом **TOO Oink Tech Ltd Co**

</div>

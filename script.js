/* ========================================
   FUN RUSSIA CRMP - Main JavaScript
   ======================================== */

// Server Configuration
const SERVER_CONFIG = {
    ip: '188.127.241.74',
    port: 4455,
    maxPlayers: 500,
    apiEndpoint: '/api/v1/server/status'
};

// ========================================
// Mobile Navigation
// ========================================
const burger = document.querySelector('.burger');
const navLinks = document.querySelector('.nav-links');

if (burger) {
    burger.addEventListener('click', () => {
        navLinks.classList.toggle('active');
        burger.classList.toggle('toggle');
    });
}

// Close mobile menu when clicking on a link
document.querySelectorAll('.nav-links a').forEach(link => {
    link.addEventListener('click', () => {
        navLinks.classList.remove('active');
    });
});

// ========================================
// Smooth Scroll for Navigation Links
// ========================================
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
        }
    });
});

// ========================================
// Header Scroll Effect
// ========================================
const header = document.querySelector('.header');
let lastScroll = 0;

window.addEventListener('scroll', () => {
    const currentScroll = window.pageYOffset;
    
    if (currentScroll > 100) {
        header.style.background = 'rgba(13, 18, 22, 0.98)';
        header.style.boxShadow = '0 5px 20px rgba(0, 0, 0, 0.3)';
    } else {
        header.style.background = 'rgba(13, 18, 22, 0.95)';
        header.style.boxShadow = 'none';
    }
    
    lastScroll = currentScroll;
});

// ========================================
// Server Monitoring
// ========================================
async function fetchServerStatus() {
    const statusIndicator = document.getElementById('statusIndicator');
    const statusText = document.getElementById('statusText');
    const onlineCount = document.getElementById('onlineCount');
    const onlineProgress = document.getElementById('onlineProgress');
    const onlinePercent = document.getElementById('onlinePercent');
    const uptimeStat = document.getElementById('uptimeStat');
    const pingStat = document.getElementById('pingStat');

    try {
        // Try to fetch from API endpoint first
        const response = await fetch(SERVER_CONFIG.apiEndpoint);
        
        if (!response.ok) {
            throw new Error('API not available');
        }
        
        const data = await response.json();
        updateServerStatus(data);
        
    } catch (error) {
        console.log('API not available, using fallback method');
        // Fallback: Simulate server status (in production, you'd use a backend proxy)
        simulateServerStatus();
    }
}

function simulateServerStatus() {
    // This simulates real server data
    // In production, replace with actual API call to your game server
    const simulatedData = {
        online: Math.floor(Math.random() * 200) + 50, // Random between 50-250
        maxPlayers: SERVER_CONFIG.maxPlayers,
        status: 'online',
        uptime: generateRandomUptime(),
        ping: Math.floor(Math.random() * 50) + 10
    };
    
    updateServerStatus(simulatedData);
}

function generateRandomUptime() {
    const days = Math.floor(Math.random() * 30);
    const hours = Math.floor(Math.random() * 24);
    return `${days}d ${hours}h`;
}

function updateServerStatus(data) {
    const statusIndicator = document.getElementById('statusIndicator');
    const statusText = document.getElementById('statusText');
    const onlineCount = document.getElementById('onlineCount');
    const onlineProgress = document.getElementById('onlineProgress');
    const onlinePercent = document.getElementById('onlinePercent');
    const uptimeStat = document.getElementById('uptimeStat');
    const pingStat = document.getElementById('pingStat');

    if (data.status === 'online') {
        statusIndicator.className = 'status-indicator online';
        statusText.textContent = 'Онлайн';
        statusText.style.color = 'var(--success-color)';
    } else {
        statusIndicator.className = 'status-indicator offline';
        statusText.textContent = 'Оффлайн';
        statusText.style.color = 'var(--danger-color)';
    }

    onlineCount.textContent = `${data.online}/${data.maxPlayers}`;
    
    const percent = Math.round((data.online / data.maxPlayers) * 100);
    onlineProgress.style.width = `${percent}%`;
    onlinePercent.textContent = percent;

    if (data.uptime) {
        uptimeStat.textContent = data.uptime;
    }
    
    if (data.ping) {
        pingStat.textContent = `${data.ping} ms`;
    }
}

function refreshServerStatus() {
    const refreshBtn = document.querySelector('.refresh-btn');
    refreshBtn.style.transform = 'rotate(180deg)';
    
    setTimeout(() => {
        fetchServerStatus();
        refreshBtn.style.transform = 'rotate(0deg)';
    }, 500);
}

// Copy IP to clipboard
function copyIp() {
    const ip = `${SERVER_CONFIG.ip}:${SERVER_CONFIG.port}`;
    navigator.clipboard.writeText(ip).then(() => {
        const copySuccess = document.getElementById('copySuccess');
        copySuccess.classList.add('show');
        
        setTimeout(() => {
            copySuccess.classList.remove('show');
        }, 2000);
    }).catch(err => {
        console.error('Failed to copy IP:', err);
        // Fallback for older browsers
        const textArea = document.createElement('textarea');
        textArea.value = ip;
        document.body.appendChild(textArea);
        textArea.select();
        document.execCommand('copy');
        document.body.removeChild(textArea);
        
        const copySuccess = document.getElementById('copySuccess');
        copySuccess.classList.add('show');
        
        setTimeout(() => {
            copySuccess.classList.remove('show');
        }, 2000);
    });
}

// Auto-refresh server status every 30 seconds
setInterval(fetchServerStatus, 30000);

// ========================================
// Admin Panel Tabs
// ========================================
const tabButtons = document.querySelectorAll('.tab-btn');
const tabContents = document.querySelectorAll('.tab-content');

tabButtons.forEach(button => {
    button.addEventListener('click', () => {
        const tabId = button.getAttribute('data-tab');
        
        // Remove active class from all buttons and contents
        tabButtons.forEach(btn => btn.classList.remove('active'));
        tabContents.forEach(content => content.classList.remove('active'));
        
        // Add active class to clicked button and corresponding content
        button.classList.add('active');
        document.getElementById(`${tabId}-tab`).classList.add('active');
    });
});

// ========================================
// Script Form Submission
// ========================================
const scriptForm = document.getElementById('scriptForm');

if (scriptForm) {
    scriptForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const scriptName = document.getElementById('scriptName').value;
        const scriptCode = document.getElementById('scriptCode').value;
        const scriptDescription = document.getElementById('scriptDescription').value;
        
        const scriptData = {
            name: scriptName,
            code: scriptCode,
            description: scriptDescription
        };
        
        try {
            // Send to API
            const response = await fetch('/api/v1/scripts/upload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${localStorage.getItem('adminToken')}`
                },
                body: JSON.stringify(scriptData)
            });
            
            if (response.ok) {
                // Add to scripts list
                const scriptsList = document.getElementById('scriptsList');
                const scriptItem = document.createElement('div');
                scriptItem.className = 'script-item';
                scriptItem.innerHTML = `
                    <span class="script-name"><i class="fas fa-file-code"></i> ${scriptName}</span>
                    <span class="script-status active">Активен</span>
                `;
                scriptsList.appendChild(scriptItem);
                
                // Clear form
                scriptForm.reset();
                
                // Show success message
                alert('Скрипт успешно загружен!');
            } else {
                throw new Error('Failed to upload script');
            }
        } catch (error) {
            console.error('Error uploading script:', error);
            // For demo purposes, still add to list
            const scriptsList = document.getElementById('scriptsList');
            const scriptItem = document.createElement('div');
            scriptItem.className = 'script-item';
            scriptItem.innerHTML = `
                <span class="script-name"><i class="fas fa-file-code"></i> ${scriptName}</span>
                <span class="script-status active">Активен</span>
            `;
            scriptsList.appendChild(scriptItem);
            scriptForm.reset();
            alert('Скрипт добавлен (демо режим)!');
        }
    });
}

// ========================================
// Function Form Submission
// ========================================
const functionForm = document.getElementById('functionForm');

if (functionForm) {
    functionForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const functionName = document.getElementById('functionName').value;
        const functionType = document.getElementById('functionType').value;
        const functionCode = document.getElementById('functionCode').value;
        const functionParams = document.getElementById('functionParams').value;
        
        const functionData = {
            name: functionName,
            type: functionType,
            code: functionCode,
            parameters: functionParams ? JSON.parse(functionParams) : {}
        };
        
        try {
            // Send to API
            const response = await fetch('/api/v1/functions/add', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${localStorage.getItem('adminToken')}`
                },
                body: JSON.stringify(functionData)
            });
            
            if (response.ok) {
                // Clear form
                functionForm.reset();
                
                // Show success message
                alert('Функция успешно добавлена!');
            } else {
                throw new Error('Failed to add function');
            }
        } catch (error) {
            console.error('Error adding function:', error);
            functionForm.reset();
            alert('Функция добавлена (демо режим)!');
        }
    });
}

// ========================================
// API Documentation Navigation
// ========================================
const apiMenuLinks = document.querySelectorAll('.api-menu a');
const apiBlocks = document.querySelectorAll('.api-block');

apiMenuLinks.forEach(link => {
    link.addEventListener('click', (e) => {
        e.preventDefault();
        
        const targetId = link.getAttribute('href').substring(1);
        
        // Remove active class from all links and blocks
        apiMenuLinks.forEach(l => l.classList.remove('active'));
        apiBlocks.forEach(block => block.classList.remove('active'));
        
        // Add active class to clicked link and corresponding block
        link.classList.add('active');
        document.getElementById(targetId).classList.add('active');
    });
});

// ========================================
// Animation on Scroll
// ========================================
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -50px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add('animate-in');
            observer.unobserve(entry.target);
        }
    });
}, observerOptions);

// Observe elements for animation
document.querySelectorAll('.about-card, .feature-item, .mode-card, .download-card').forEach(el => {
    el.style.opacity = '0';
    el.style.transform = 'translateY(30px)';
    el.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
    observer.observe(el);
});

// Add animate-in styles dynamically
const style = document.createElement('style');
style.textContent = `
    .animate-in {
        opacity: 1 !important;
        transform: translateY(0) !important;
    }
`;
document.head.appendChild(style);

// ========================================
// Counter Animation for Stats
// ========================================
function animateCounter(element, target, duration = 2000) {
    let start = 0;
    const increment = target / (duration / 16);
    
    const timer = setInterval(() => {
        start += increment;
        if (start >= target) {
            element.textContent = target;
            clearInterval(timer);
        } else {
            element.textContent = Math.floor(start);
        }
    }, 16);
}

// ========================================
// Parallax Effect for Hero Section
// ========================================
const hero = document.querySelector('.hero');

if (hero) {
    window.addEventListener('scroll', () => {
        const scrolled = window.pageYOffset;
        const heroContent = document.querySelector('.hero-content');
        
        if (heroContent && scrolled < window.innerHeight) {
            heroContent.style.transform = `translateY(${scrolled * 0.3}px)`;
            heroContent.style.opacity = 1 - (scrolled / window.innerHeight);
        }
    });
}

// ========================================
// Initialize on DOM Load
// ========================================
document.addEventListener('DOMContentLoaded', () => {
    // Fetch initial server status
    fetchServerStatus();
    
    // Add loaded class to body for CSS animations
    document.body.classList.add('loaded');
    
    // Console welcome message
    console.log('%c🎮 FUN RUSSIA CRMP', 'font-size: 24px; font-weight: bold; color: #ff4757;');
    console.log('%cДобро пожаловать на официальный сайт!', 'font-size: 14px; color: #ffa502;');
    console.log('%cВерсия сайта: 1.0.0', 'font-size: 12px; color: #a4b0be;');
});

// ========================================
// Player Search Functionality
// ========================================
const searchInput = document.querySelector('.search-input');

if (searchInput) {
    searchInput.addEventListener('input', (e) => {
        const searchTerm = e.target.value.toLowerCase();
        const tableRows = document.querySelectorAll('.players-table tbody tr');
        
        tableRows.forEach(row => {
            const playerName = row.querySelector('td:nth-child(2)').textContent.toLowerCase();
            
            if (playerName.includes(searchTerm)) {
                row.style.display = '';
            } else {
                row.style.display = 'none';
            }
        });
    });
}

// ========================================
// Local Storage for Admin Token
// ========================================
// This is a placeholder for actual authentication
function setAdminToken(token) {
    localStorage.setItem('adminToken', token);
}

function getAdminToken() {
    return localStorage.getItem('adminToken');
}

function removeAdminToken() {
    localStorage.removeItem('adminToken');
}

// Export functions for external use
window.FUNRUSSIA = {
    copyIp,
    refreshServerStatus,
    fetchServerStatus,
    setAdminToken,
    getAdminToken,
    removeAdminToken,
    SERVER_CONFIG
};

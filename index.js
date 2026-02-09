import midi from "easymidi";
import { SerialPort } from "serialport";

// ⚠️ 設定電子琴名稱 (就是改這邊 通常沒換琴 只需要改一次)
const DEVICE_NAME = "2- Digital Keyboard-1"; 

// --- 自動偵測 Arduino Port 的函數 ---
async function autoDetectArduino() {
    console.log("🔍 正在掃描 USB 裝置...");
    const ports = await SerialPort.list();
    
    // 尋找特徵：廠商名稱含 'Arduino' 或 VendorID 為 '2341' (Arduino原廠ID)
    const arduino = ports.find(port => 
        (port.manufacturer && port.manufacturer.includes("Arduino")) ||
        (port.vendorId && port.vendorId.toLowerCase() === "2341") || 
        // 針對某些 UNO R4 WiFi 可能顯示的特殊名稱
        (port.friendlyName && port.friendlyName.includes("Arduino"))
    );

    if (arduino) {
        console.log(`✅ 找到 Arduino！位於: ${arduino.path} (${arduino.manufacturer})`);
        return arduino.path;
    } else {
        return null;
    }
}

// --- 主程式 (改用 async 結構) ---
(async () => {
    // 1. 自動取得 Port
    const detectedPort = await autoDetectArduino();

    if (!detectedPort) {
        console.error("❌ 找不到 Arduino！請確認：\n1. USB 線有插好嗎？\n2. Arduino IDE 視窗有關閉嗎？(避免佔用)");
        process.exit(1);
    }

    // 2. 連接 Arduino (使用自動找到的 Port)
    const port = new SerialPort({ path: detectedPort, baudRate: 115200 });

    port.on('open', () => {
        console.log(`🔌 連線建立成功！(BaudRate: 115200)`);
    });

    port.on('error', (err) => {
        console.error('❌ Arduino 連線錯誤:', err.message);
    });

    // 3. 連接電子琴
    let input;
    try {
        input = new midi.Input(DEVICE_NAME);
        console.log(`🎹 電子琴已連線: ${DEVICE_NAME}`);
    } catch (e) {
        console.error(`❌ 找不到電子琴: ${DEVICE_NAME}\n請確認電子琴電源已開啟且 USB 已連接。`);
        process.exit(1);
    }

    // === 燕雲十六聲 映射表 (保留你原本的設定) ===
    const keyMap = {
        // 低音 (Z-M)
        48: { key: 'z', mod: 0 }, 49: { key: 'z', mod: 1 }, 50: { key: 'x', mod: 0 },
        51: { key: 'c', mod: 2 }, 52: { key: 'c', mod: 0 }, 53: { key: 'v', mod: 0 },
        54: { key: 'v', mod: 1 }, 55: { key: 'b', mod: 0 }, 56: { key: 'b', mod: 1 },
        57: { key: 'n', mod: 0 }, 58: { key: 'm', mod: 2 }, 59: { key: 'm', mod: 0 },

        // 中音 (A-J)
        60: { key: 'a', mod: 0 }, 61: { key: 'a', mod: 1 }, 62: { key: 's', mod: 0 },
        63: { key: 'd', mod: 2 }, 64: { key: 'd', mod: 0 }, 65: { key: 'f', mod: 0 },
        66: { key: 'f', mod: 1 }, 67: { key: 'g', mod: 0 }, 68: { key: 'g', mod: 1 },
        69: { key: 'h', mod: 0 }, 70: { key: 'j', mod: 2 }, 71: { key: 'j', mod: 0 },

        // 高音 (Q-U)
        72: { key: 'q', mod: 0 }, 73: { key: 'q', mod: 1 }, 74: { key: 'w', mod: 0 },
        75: { key: 'e', mod: 2 }, 76: { key: 'e', mod: 0 }, 77: { key: 'r', mod: 0 },
        78: { key: 'r', mod: 1 }, 79: { key: 't', mod: 0 }, 80: { key: 't', mod: 1 },
        81: { key: 'y', mod: 0 }, 82: { key: 'u', mod: 2 }, 83: { key: 'u', mod: 0 },
    };

    console.log("系統就緒！直接彈奏即可！");

    input.on("noteon", (msg) => {
        const { note, velocity, channel } = msg;

        // 過濾掉疊音 (只聽 Channel 0)
        if (channel !== 0) return;

        if (velocity > 0 && keyMap[note]) {
            const action = keyMap[note];
            const payload = Buffer.from([action.mod, action.key.charCodeAt(0)]);

            port.write(payload, (err) => {
                if (err) return console.log('Write Error: ', err.message);
                
                let prefix = "⚪";
                if (action.mod === 1) prefix = "⬆️";
                if (action.mod === 2) prefix = "⬇️";
                console.log(`${prefix} ${action.key.toUpperCase()}`);
            });
        }
    });

})();
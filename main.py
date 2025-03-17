import machine
import time

pwm_pin = 18  # 連接到信號的引腳
control_pin = 8  # 連接到控制的引腳

# 設置引腳為輸入模式
pin = machine.Pin(pwm_pin, machine.Pin.IN)
control = machine.Pin(control_pin, machine.Pin.OUT)

# 用於存儲脈衝計數器、總金額、時間和上次脈衝時間的變量
total_amount = 0  # 總金額
pulse_count = 0  # 當前脈衝計數
last_pulse_time = time.ticks_ms()  # 上一次脈衝的時間
control.value(1)

def irq_handler(pin):
    global pulse_count, last_pulse_time
    control.value(0)
    last_pulse_time = time.ticks_ms()  # 更新上一次脈衝的時間
    print(last_pulse_time)
    if pin.value() == 1:  # 當引腳值為高電平（上升沿）
        pulse_count += 1  # 當前脈衝計數增加 1

def calculate_amount(pulse_count):
    # 根據脈衝數量計算金額
    if pulse_count == 1:
        return 1
    elif pulse_count == 2:
        return 5
    elif pulse_count == 3:
        return 10
    elif pulse_count == 4:
        return 50
    else:
        return 0

# 配置引腳中斷，僅在上升沿觸發中斷
pin.irq(trigger=machine.Pin.IRQ_RISING | machine.Pin.IRQ_FALLING, handler=irq_handler)

while True:
    current_time = time.ticks_ms()  # 獲取當前時間
    if time.ticks_diff(current_time, last_pulse_time) > 105:  # 如果距上次脈衝時間已超過 105 毫秒
        if pulse_count > 0:
            # 計算金額並更新總金額
            amount = calculate_amount(pulse_count)
            total_amount += amount

            # 打印當前總金額
            print(f"Total Amount: {total_amount} 元")
            control.value(1)
            # 重置脈衝計數器
            pulse_count = 0
        
        # 更新最後的脈衝時間
        last_pulse_time = current_time
    
    # 確保主循環檢查時間的間隔
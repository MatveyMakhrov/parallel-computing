import matplotlib.pyplot as plt

# Исходные данные ti
ti = [90.2440, 76.4550, 73.3974, 73.2878, 72.5012, 72.3140, 73.8209, 80.0619, 77.9432, 72.4867]

# Метки по оси X
x_labels = ['ti', 't_min', 't_avg(T)', "t_avg(T')", "t_avg(T''')"]

# Значения усреднённых точек
y_values = [
    None,        # ti будет нарисован отдельно
    72.3140,     # t_min
    76.2513,     # t_avg(T)
    74.9810,     # t_avg(T')
    72.8669      # t_avg(T''')
]

# Доверительные интервалы (для усреднённых точек)
y_err = [
    None,
    None,
    [[76.2513 - 72.7954], [79.7071 - 76.2513]],  # t_avg(T)
    [[74.9810 - 73.3333], [76.6287 - 74.9810]],  # t_avg(T')
    [[72.8669 - 72.2140], [73.3974 - 72.8669]]   # t_avg(T''')
]

plt.figure(figsize=(10, 6))

# Отдельно рисуем исходные данные ti
plt.scatter([0]*len(ti), ti, color='blue', label='ti', zorder=3)

# Рисуем усреднённые точки с доверительными интервалами
for i in range(1, len(y_values)):
    y = y_values[i]
    if y_err[i] is not None:
        lower = y - y_err[i][0][0]
        upper = y + y_err[i][1][0]
        plt.errorbar(i, y, yerr=[[y - lower], [upper - y]], fmt='o', color='red', capsize=5, label='_nolegend_')
    else:
        plt.scatter(i, y, color='red')

# Подписи по оси X
plt.xticks(range(len(x_labels)), x_labels)

plt.ylabel('Время выполнения, мс')
plt.title('Сравнение исходных данных и усреднённых значений с доверительными интервалами')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.legend(['ti', 'Средние значения с доверительным интервалом'])
plt.show()
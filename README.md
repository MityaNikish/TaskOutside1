# Задача о работе ударной трубы
___

![image](https://github.com/user-attachments/assets/9f5ee07a-0800-4281-9732-f3f33b898251)
![image](https://github.com/user-attachments/assets/524daf48-f527-463d-9e66-b023d28f4086)
![image](https://github.com/user-attachments/assets/4d1d8ced-8fdf-4d77-97cd-9408dee7c803)
![image](https://github.com/user-attachments/assets/a1cc1a8c-6f12-4dc6-bd12-53ed59e6866b)

___
# Изменение давления в трубе
![output_p](https://github.com/user-attachments/assets/5cb34ede-407d-4bf1-9ef6-6e53d102c6f2)
___
# Изменение плотности в трубе
![output_ro](https://github.com/user-attachments/assets/c8816cea-c14c-4f92-b8f8-958ac9b903b0)
__
# Изменение скорости в трубе
![output_v](https://github.com/user-attachments/assets/f739aa0a-e7d8-40a9-b589-c02fa708ddd7)
__
# Код для отрисовки:
```Python
import numpy as np
import matplotlib.pyplot as plt
import imageio
from moviepy.editor import ImageSequenceClip

# Функция для чтения бинарного файла и преобразования его в массив
def read_raw_file(file_name, shape):
    with open(file_name, "rb") as f:
        data = np.fromfile(f, dtype=np.float64)  # Чтение как double
        return data.reshape(shape)  # Преобразование в массив

# Функция для сохранения массива как изображения
def save_image(array, h, array_shape, file_name):
    x = np.array([i * h for i in range(array_shape)])
    plt.plot(x, array)
    plt.savefig(file_name)
    plt.close()

# Функция для создания GIF
def create_gif(image_files, output_gif, duration=0.5):
    images = [imageio.imread(img) for img in image_files]
    imageio.mimsave(output_gif, images, duration=duration)  # Создание GIF, duration - время между кадрами

# Функция для создания видео MP4
def create_video(image_files, output_video, fps=1):
    clip = ImageSequenceClip(image_files, fps=fps)
    clip.write_videofile(output_video, codec="libx264")

# Основная функция
def main():
    h = 0.1
    Lx = 100.0
    n_save = 100
    N = 34
    
    # Параметры
    path = "C:\\Users\\DN_Kr\\source\\repos\\Task\\TaskOutside1\\treatment\\v\\"
    path_save = "C:\\Users\\DN_Kr\\OneDrive\\Desktop\\Maga\\C++\\Task4\\"
    num_files = [i for i in range(N)]
    array_shape = int(Lx / h) + 1

    image_files = []
    fps = 10 # fps - количество кадров в секунду


    # Чтение файлов, сохранение изображений
    for i in num_files:
        file_name = path + f"output_{i}.raw"
        array = read_raw_file(file_name, array_shape)
        image_file = path + f"image_{i}.png"
        save_image(array, h, array_shape, image_file)
        image_files.append(image_file)

    # Создание GIF
    create_gif(image_files, path_save + "output.gif", 1 / fps)

    # Создание MP4 видео
    create_video(image_files, path_save + "output.mp4", fps)  

if __name__ == "__main__":
    main()
```

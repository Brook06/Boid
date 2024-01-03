import numpy as np

with open("coordinates.txt", "r") as file:
    lines = file.readlines()
    lines = [line.strip() for line in lines]

lines = [line.replace(",", ".") for line in lines]

dimensions = [float(n) for n in lines[0].split(" ")]

n_frames = len(lines) - 1
n_coordinates = len([float(n) for n in lines[1].split(" ")])
if n_coordinates % 2:
    raise ValueError("Number of coordinates must be even.")
n_boids = int(n_coordinates / 2)

print("Number of Frames : ", n_frames)
print("Number of Boids: ", n_boids)

frames = []
for frame in range(n_frames):
    coordinates = [float(n) for n in lines[frame + 1].split(" ")]
    frames.append(coordinates)
frames = np.array(frames).reshape(n_frames, n_boids, 2)


import random
import matplotlib.pyplot as plt
import matplotlib.animation as animation

COLORS = ["blue", "orange", "green", "red", "purple", "brown", "teal", "gray"]
# COLORS = [
#     "#001219",
#     "#005F73",
#     "#0A9396",
#     "#94D2BD",
#     "#E9D8A6",
#     "#EE9B00",
#     "#CA6702",
#     "#BB3E03",
#     "#AE2012",
#     "#9B2226",
# ]

c_boids = [COLORS[random.randint(0, len(COLORS) - 1)] for _ in range(n_boids)]
c_boids[0] = "black"

width = dimensions[1] - dimensions[0]
height = dimensions[3] - dimensions[2]

max_value = max(width, height)
width *= 5 / max_value
height *= 5 / max_value

# Create the figure and axis
fig, ax = plt.subplots(figsize=(width, height))

# Set the limits of the plot
ax.set_xlim(dimensions[0], dimensions[1])
ax.set_ylim(dimensions[2], dimensions[3])

# Define the update function for the animation
# Create the initial scatter plot
scatter = ax.scatter([], [], s=32)


# Define the update function for the animation
def update(frame):
    x = []
    y = []
    c = []  # List to store random colors for each boid

    # Get the coordinates for the current frame
    for boid in range(n_boids):
        x.append(frames[frame][boid][0])
        y.append(frames[frame][boid][1])

    # Update the scatter plot with the new coordinates and colors
    scatter.set_offsets(list(zip(x, y)))
    scatter.set_color(c_boids)

    return (scatter,)


interval = int(input("Enter the interval between frames (in ms): "))
ani = animation.FuncAnimation(fig, update, frames=n_frames, interval=interval)
plt.show()

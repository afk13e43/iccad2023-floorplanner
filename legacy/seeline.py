import matplotlib.pyplot as plt
import random

# Read file into the polygons list

polygons = []
connects = []
sma = 0
fma = 0
cnt = 0
coor_cnt = 0
tmp_coors = 0
tmp_coors_list = []
polygons = [[]]  # Don't store anything in the first index
with open('coor.txt', 'r') as f:
    for line in f.readlines():
        # Extract sma and fma from the first line and skip it
        if cnt == 0:
            line = line.strip().split(" ")
            fma = int(line[1])
            cnt += 1
            continue
        if line[0:3] == 'mod':
            line = line.strip().split(" ")
            tmp_coors = int(line[1])
            tmp_coors_list = []
            continue

        if tmp_coors > 0:
            line = line.strip().split(" ")
            x = float(line[0])
            y = float(line[1])
            tmp_coors -= 1
            tmp_coors_list.append((x, y))
            if tmp_coors == 0:
                polygons.append(tmp_coors_list)
                tmp_coors_list = []

1# Create coordinate grid
fig, ax = plt.subplots(figsize=(9, 7),dpi=200)
# Set coordinate grid range
case = int(input('case:'))
if case == 1:
    ax.set_xlim(0, 13267)
    ax.set_ylim(0, 13267)
elif case == 2:
    ax.set_xlim(0, 3000)
    ax.set_ylim(0, 3000)
elif case == 3:
    ax.set_xlim(0, 4000)
    ax.set_ylim(0, 4000)
elif case == 4:
    ax.set_xlim(0, 6995)
    ax.set_ylim(0, 6995)
elif case == 5:
    ax.set_xlim(0, 4620)
    ax.set_ylim(0, 4620)
elif case == 6:
    ax.set_xlim(0, 4000)
    ax.set_ylim(0, 4000)
else:
    ax.set_xlim(0, int(input('x:')))
    ax.set_ylim(0, int(input('y:')))

# Draw multiple polygons
centers = []  # Store the center coordinates of each polygon
for i, coords in enumerate(polygons[1:]):  # Iterate over polygons list starting from index 1
    # Generate random RGB values
    r = random.random()
    g = random.random()
    b = random.random()
    color = (r, g, b)

    # Check if it is a "fix module", if so, set the border color to red
    if i < sma or i >= sma + fma:
        edgecolor = 'darkred'
        linewidth = 1
    else:
        edgecolor = 'darkblue'
        linewidth = 1

    polygon = plt.Polygon(coords, closed=True, facecolor=color, edgecolor=edgecolor, alpha=0.4)
    polygon.set_linewidth(linewidth)
    ax.add_patch(polygon)

    # Calculate the center coordinates of the minimum bounding rectangle
    x_coords = [coord[0] for coord in coords]
    y_coords = [coord[1] for coord in coords]
    center_x = (min(x_coords) + max(x_coords)) / 2
    center_y = (min(y_coords) + max(y_coords)) / 2
    centers.append((center_x, center_y))
    plt.text(center_x, center_y, str(i + 1), color='white', ha='center', va='center')

# Draw connection lines
weights = []
with open('cnt.txt', 'r') as f:
    for line in f.readlines():
        line = line.strip().split(" ")
        idx1 = int(line[0])
        idx2 = int(line[1])
        weight = int(line[2])
        weights.append((idx1, idx2, weight))

line_offsets = {}  # Store line offsets
offset = 10  # Initial line offset

for i in range(len(weights)):
    idx1, idx2, weight = weights[i]
    center1 = centers[idx1 - 1]
    center2 = centers[idx2 - 1]

    # Calculate the start and end points of the line segment
    x1, y1 = center1
    x2, y2 = center2

    # Check if the line segment overlaps w3ith existing ones
    if (x1, y1, x2, y2) in line_offsets:
        offset += 20  # Increase the offset if it overlaps

    # Update the line offset
    line_offsets[(x1, y1, x2, y2)] = offset

    # Set the color of the line segment to black
    color = 'black'

    # Draw the connection line
    plt.plot([x1, x2], [y1, y2], color=color, linewidth=weight / 500)

    offset += 20  # Increase the offset


def on_scroll(event):
    if event.button == 'up':
        scale_factor = 1.1
    elif event.button == 'down':
        scale_factor = 0.9
    else:
        scale_factor = 1.0

    ax.set_xlim(ax.get_xlim()[0] * scale_factor, ax.get_xlim()[1] * scale_factor)
    ax.set_ylim(ax.get_ylim()[0] * scale_factor, ax.get_ylim()[1] * scale_factor)

    plt.draw()


# Register the scroll event handlerpby
fig.canvas.mpl_connect('scroll_event', on_scroll)

ax.set_aspect('equal')
plt.show()
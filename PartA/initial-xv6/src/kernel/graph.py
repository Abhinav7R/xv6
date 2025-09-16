import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the text file into a DataFrame
file_path = 'a.txt'
df = pd.read_csv(file_path, delimiter=' ', header=None, names=['Process', 'Ticks', 'Queue'])

# Define colors for the six processes
colors = ['b', 'g', 'r', 'c', 'm', 'y']

# Create a figure and axis
plt.figure(figsize=(10, 6))
ax = plt.subplot(111)

# Loop through each process and plot its data points as a line graph
for process in range(9, 14):
    process_data = df[df['Process'] == process]
    ticks = process_data['Queue']
    queue_numbers = process_data['Ticks']
    
    ax.plot(ticks, queue_numbers, label=f'Process {process-4}', color=colors[process - 9], linestyle='-')

plt.xlabel('Number of Ticks')
plt.ylabel('Queue Number')
plt.title('Queue Number vs. Number of Ticks')
plt.legend()
plt.ylim(-0.5, 3.5)  # Set y-axis limits from -0.5 to 3.5
plt.grid(True)

plt.savefig('line_graph.png', dpi=300, bbox_inches='tight')

plt.show()
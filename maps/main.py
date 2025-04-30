import tkinter as tk
from tkinter import filedialog

class MapEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("Ant Map Editor")
        frame = tk.Frame(root)
        frame.pack(fill=tk.BOTH, expand=True)
        self.canvas = tk.Canvas(frame, width=800, height=600, bg='white', scrollregion=(0, 0, 2000, 2000))
        self.canvas.pack()

        self.nodes = {}         # name → (x, y)
        self.edges = set()      # set of frozenset([a, b])
        self.node_id = 1
        self.selected_node = None
        self.hover_node = None
        self.hover_edge = None
        self.special_nodes = {}    # name → "start" / "end"
        self.node_tags = {}        # name → (oval_id, text_id)
        self.edge_lines = {}       # frozenset([a, b]) → canvas line id

        self.canvas.bind("<Button-1>", self.create_node)
        self.canvas.bind("<Button-3>", self.click_node)
        self.canvas.bind("<Motion>", self.track_hover)
        self.root.bind("<s>", self.mark_start)
        self.root.bind("<d>", self.mark_end)
        self.root.bind("<n>", self.mark_normal)
        self.root.bind("<e>", self.export_map)
        self.root.bind("<BackSpace>", self.delete_selected)
        self.root.bind("<l>", self.load_map)

        # Inside __init__:

        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        scroll_y = tk.Scrollbar(frame, orient=tk.VERTICAL, command=self.canvas.yview)
        scroll_y.pack(side=tk.RIGHT, fill=tk.Y)
        scroll_x = tk.Scrollbar(root, orient=tk.HORIZONTAL, command=self.canvas.xview)
        scroll_x.pack(side=tk.BOTTOM, fill=tk.X)

        self.canvas.configure(yscrollcommand=scroll_y.set, xscrollcommand=scroll_x.set)

        self.canvas.bind("<ButtonPress-2>", self.start_pan)
        self.canvas.bind("<B2-Motion>", self.do_pan)


    def start_pan(self, event):
        self.canvas.scan_mark(event.x, event.y)

    def do_pan(self, event):
        self.canvas.scan_dragto(event.x, event.y, gain=1)


    def create_node(self, event, name=None, pos=None):
        if not name:
            name = f"R{self.node_id}"
            self.node_id += 1
        if not pos:
            pos = (event.x, event.y)
        self.nodes[name] = pos
        color = {"start": "green", "end": "red"}.get(self.special_nodes.get(name), "lightblue")
        oval = self.canvas.create_oval(pos[0]-10, pos[1]-10, pos[0]+10, pos[1]+10, fill=color, tags=name)
        text = self.canvas.create_text(pos[0], pos[1], text=name, tags=name)
        self.node_tags[name] = (oval, text)

    def click_node(self, event):
        clicked = self.find_node_at(event.x, event.y)
        if clicked:
            if self.selected_node and self.selected_node != clicked:
                edge = frozenset([self.selected_node, clicked])
                if edge not in self.edges:
                    self.edges.add(edge)
                    self.draw_edge(*edge)
                self.selected_node = None
            else:
                self.selected_node = clicked
                self.hover_node = clicked

    def track_hover(self, event):
        self.hover_node = self.find_node_at(event.x, event.y)
        self.hover_edge = self.find_edge_near(event.x, event.y)

    def find_node_at(self, x, y):
        for name, (nx, ny) in self.nodes.items():
            if abs(x - nx) <= 10 and abs(y - ny) <= 10:
                return name
        return None

    def find_edge_near(self, x, y, tolerance=5):
        for edge in self.edges:
            a, b = list(edge)
            x1, y1 = self.nodes[a]
            x2, y2 = self.nodes[b]
            if self.point_near_line(x, y, x1, y1, x2, y2, tolerance):
                return edge
        return None

    def point_near_line(self, px, py, x1, y1, x2, y2, tol):
        if x1 == x2 and y1 == y2:
            return False
        dx = x2 - x1
        dy = y2 - y1
        length_sq = dx**2 + dy**2
        t = max(0, min(1, ((px - x1) * dx + (py - y1) * dy) / length_sq))
        proj_x = x1 + t * dx
        proj_y = y1 + t * dy
        dist_sq = (px - proj_x)**2 + (py - proj_y)**2
        return dist_sq <= tol**2

    def draw_edge(self, a, b):
        x1, y1 = self.nodes[a]
        x2, y2 = self.nodes[b]
        line = self.canvas.create_line(x1, y1, x2, y2, fill="black")
        self.edge_lines[frozenset([a, b])] = line

    def mark_start(self, event=None):
        if self.hover_node:
            self.special_nodes[self.hover_node] = "start"
            self.canvas.itemconfig(self.node_tags[self.hover_node][0], fill="green")

    def mark_end(self, event=None):
        if self.hover_node:
            self.special_nodes[self.hover_node] = "end"
            self.canvas.itemconfig(self.node_tags[self.hover_node][0], fill="red")

    def mark_normal(self, event=None):
        if self.hover_node and self.hover_node in self.special_nodes:
            del self.special_nodes[self.hover_node]
            self.canvas.itemconfig(self.node_tags[self.hover_node][0], fill="lightblue")

    def delete_selected(self, event=None):
        if self.hover_node:
            name = self.hover_node
            to_remove = [e for e in self.edges if name in e]
            for edge in to_remove:
                self.canvas.delete(self.edge_lines[edge])
                del self.edge_lines[edge]
                self.edges.remove(edge)
            oval, text = self.node_tags[name]
            self.canvas.delete(oval)
            self.canvas.delete(text)
            self.nodes.pop(name)
            self.node_tags.pop(name)
            self.special_nodes.pop(name, None)
            if self.selected_node == name:
                self.selected_node = None
            self.hover_node = None
        elif self.hover_edge:
            edge = self.hover_edge
            self.canvas.delete(self.edge_lines[edge])
            del self.edge_lines[edge]
            self.edges.remove(edge)
            self.hover_edge = None

    def export_map(self, event=None):
        with open("map_output.txt", "w") as f:
            f.write("1\n")
            for name, (x, y) in self.nodes.items():
                if name in self.special_nodes:
                    f.write(f"##{self.special_nodes[name]}\n")
                f.write(f"{name} {x} {y}\n")
            for edge in self.edges:
                a, b = list(edge)
                f.write(f"{a}-{b}\n")
        print("Map exported to map_output.txt")

    def load_map(self, event=None):
        path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if not path:
            return

        # Clear current map
        for line in self.edge_lines.values():
            self.canvas.delete(line)
        for oval, text in self.node_tags.values():
            self.canvas.delete(oval)
            self.canvas.delete(text)
        self.nodes.clear()
        self.edges.clear()
        self.node_tags.clear()
        self.edge_lines.clear()
        self.special_nodes.clear()
        self.node_id = 1

        with open(path, 'r') as f:
            lines = f.readlines()

        current_type = None
        for line in lines:
            line = line.strip()
            if not line or line == "1":
                continue
            if line == "##start":
                current_type = "start"
                continue
            elif line == "##end":
                current_type = "end"
                continue
            elif '-' in line:
                a, b = line.split('-')
                edge = frozenset([a, b])
                if edge not in self.edges and a in self.nodes and b in self.nodes:
                    self.edges.add(edge)
                    self.draw_edge(a, b)
            else:
                parts = line.split()
                if len(parts) == 3:
                    name, x, y = parts[0], int(parts[1]), int(parts[2])
                    if current_type:
                        self.special_nodes[name] = current_type
                        current_type = None
                    self.create_node(None, name=name, pos=(x, y))
                    self.node_id = max(self.node_id, int(name.strip("R")) + 1 if name.startswith("R") else self.node_id)

if __name__ == "__main__":
    root = tk.Tk()
    app = MapEditor(root)
    root.mainloop()

#pragma once

#include<vector>
#include<stack>

enum class ColorN {
	white, grey, black
};

enum class ColorL {
	green, blue, red, black
};

class Node;

struct Link {
	int64_t val;
	ColorL color;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	ColorN color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(ColorN::white) {}

	void AddLink(Link* link) {
		links.push_back(link);
	}

	void ChangeColor(ColorN colorl) {
		color = colorl;
	}

	uint64_t GetNumber() {
		return number;
	}

	ColorN GetColor() {
		return color;
	}

	const std::vector<Link*>& GetLinks() {
		return links;
	}
};

class Graph{
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;

public:
	Graph(uint64_t size) : size(size){
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		Link* NewLink = new Link(val, nodes[in], nodes[out]);

		nodes[in]->AddLink(NewLink);

		/*graph.at(out).at(in) = NewLink;
		node[out]->AddLink(NewLink);*/
	}

	const std::vector<Link*>& GetAdj(Node* node) {
		return node->GetLinks();
	}

	std::vector<Node*> SearchCycle(uint64_t number) {
		std::vector<Node*> Cycle;
		std::stack<std::pair<Node*, uint64_t>> trail;

		trail.push({ nodes[number], 0 });
		nodes[number]->ChangeColor(ColorN::grey);

		while (!trail.empty()) {
			auto cur = trail.top();

			std::vector<Link*> AdjLinks = GetAdj(cur.first);

			for (auto i = cur.second; i < AdjLinks.size(); ++i) {
				if (AdjLinks[i]->node2->GetColor() == ColorN::grey) {
					while (trail.top().first != AdjLinks[i]->node2) {
						Cycle.push_back(trail.top().first);
						trail.pop();
					}

					Cycle.push_back(AdjLinks[i]->node2);

					return Cycle;
				}
				else if (AdjLinks[i]->node2->GetColor() == ColorN::white) {
					trail.top().second = i;
					trail.push({ AdjLinks[i]->node2, 0 });
					AdjLinks[i]->node2->ChangeColor(ColorN::grey);
					break;
				}
			}

			if (trail.top() == cur) {
				cur.first->ChangeColor(ColorN::black);
				trail.pop();
			}
		}

		return Cycle;
	}

	Graph ReverseGraph() {
		Graph revGraph(size);

		for (auto link : links) {
			revGraph.AddLink(link->node2->GetNumber(), link->node1->GetNumber(), link->val);
		}

		return revGraph;
	}

	void clear() {
		for (auto node : nodes) {
			node->ChangeColor(ColorN::white);
		}
	}

	std::vector<Node*> TopologicalSort(uint64_t start) {
		std::vector<Node*> answer;
		Node* cur = nodes[start];

		this->clear();
		/*To be continued*/
	}
};


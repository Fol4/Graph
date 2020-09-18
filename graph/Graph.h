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
	std::vector<Node*> node;
	std::vector<Link*> links;

public:
	Graph(uint64_t number) {
		for (uint64_t i = 0; i < number; ++i) {
			node.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		Link* NewLink = new Link(val, node[in], node[out]);

		node[in]->AddLink(NewLink);

		/*graph.at(out).at(in) = NewLink;
		node[out]->AddLink(NewLink);*/
	}

	const std::vector<Link*>& GetAdj(Node* node) {
		return node->GetLinks();
	}

	std::vector<Node*> SearchCycle(uint64_t number) {
		std::vector<Node*> Cycle;
		std::stack<std::pair<Node*, uint64_t>> trail;

		trail.push({ node[number], 0 });
		node[number]->ChangeColor(ColorN::grey);

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
};


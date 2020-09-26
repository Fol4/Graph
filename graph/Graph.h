#pragma once

#include <vector>
#include <stack>
#include <algorithm>

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

		links.push_back(NewLink);
	}

	std::vector<Node*> SearchCycle(uint64_t number) {
		std::vector<Node*> Cycle;
		std::stack<std::pair<Node*, uint64_t>> trail;

		trail.push({ nodes[number], 0 });
		nodes[number]->ChangeColor(ColorN::grey);

		while (!trail.empty()) {
			auto cur = trail.top();

			std::vector<Link*> children = cur.first->GetLinks();

			for (auto i = cur.second; i < children.size(); ++i) {
				if (children[i]->node2->GetColor() == ColorN::grey) {
					while (trail.top().first != children[i]->node2) {
						Cycle.push_back(trail.top().first);
						trail.pop();
					}

					Cycle.push_back(children[i]->node2);

					return Cycle;
				}
				else if (children[i]->node2->GetColor() == ColorN::white) {
					trail.top().second = i;
					trail.push({ children[i]->node2, 0 });
					children[i]->node2->ChangeColor(ColorN::grey);
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

	Graph reverse() {
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

	std::vector<Node*> dfs(uint64_t number) {
		std::stack<std::pair<Node*, uint64_t>> deep;
		std::vector<Node*> result;

		if (nodes[number]->GetColor() == ColorN::black) {
			return result;
		}

		nodes[number]->ChangeColor(ColorN::black);
		deep.push({ nodes[number], 0 });

		while (!deep.empty()) {
			std::vector<Link*> children = deep.top().first->GetLinks();
			std::pair<Node*, uint64_t> cur = deep.top();

			for (auto i = cur.second; i < children.size(); ++i) {
				if (children[i]->node2->GetColor() == ColorN::white) {
					deep.top().second = i + 1;
					children[i]->node2->ChangeColor(ColorN::black);
					deep.push({ children[i]->node2, 0 });
					break;
				}
			}

			if (cur.first == deep.top().first) {
				result.push_back(deep.top().first);
				deep.pop();
			}
		}

		return result;
	}

	std::vector<Node*> TopologicalSort() {
		std::vector<Node*> answer;

		clear();
		
		for (auto node : nodes) {
			std::vector<Node*> result = dfs(node->GetNumber());

			if (!result.empty()) {
				answer.insert(answer.end(), result.begin(), result.end());
			}
		}

		std::reverse(answer.begin(), answer.end());

		return answer;
	}

	std::vector<std::vector<Node*>>  findSCC() {
		std::vector<std::vector<Node*>> SCC;
		clear();

		Graph revGraph = reverse();
		std::vector<Node*> sorted = TopologicalSort();

		for (auto node : sorted) {
			std::vector<Node*> result = revGraph.dfs(node->GetNumber());

			if (!result.empty()) {
				SCC.push_back(result);
			}
		}

		return SCC;
	}
};


#pragma once
#include <iostream>
#include <vector>
#include <string>

class Map
{
public:
	Map(std::string name, Map* parent = nullptr);

	void addChild(Map* child);

	void setWLD(std::vector<int> v);
	void addWin(std::ostream& output);
	void addLoss(std::ostream& output);
	void addDraw(std::ostream& output);

	std::vector<int> getWLD() const;
	int getWins() const;
	int getLosses() const;
	int getDraws() const;
	int getGames() const;
	std::string getName() const;
	Map* getMapt() const;
	std::vector<Map*> getChildren() const;

	double getWinrate() const;

	std::string getOutput(const std::string::size_type& maplen) const;

private:
	Map* parent_ = nullptr;
	std::vector<Map*> children_ = {};
	std::string name_;

	int W_ = 0;
	int L_ = 0;
	int D_ = 0;
};
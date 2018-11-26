class Ai {
	public:
		virtual void update(std::shared_ptr<Ent> owner) = 0;
};

class PlayerAi : public Ai {
	public: 
		void update(std::shared_ptr<Ent> owner);

	protected:
		bool moveOrAttack(std::shared_ptr<Ent> owner, int tx, int ty);
};

class MobAi : public Ai {
	public:
		void update(std::shared_ptr<Ent> owner);

	protected:
		int moveCount;
		void moveOrAttack(std::shared_ptr<Ent> owner, int tx, int ty);
};
class Combat{
	public:
		float atk;
		Combat(float atk);
		void attack(std::shared_ptr<Ent> attacker, std::shared_ptr<Ent> target);
};
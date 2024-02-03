class Example7App: public BaseApp {

public:
    void sendAbandonMessage();
    virtual void sendUnicast(cPacket* msg, int destination);

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* msg) override;
    BaseScenario* scenario;

private:
    AbandonPlatoon* createAbandonMessage();
    NewFormation* createNewFormationMessage(const std::vector<int>& newPlatoonFormation);
    void handleAbandoonMessage(AbandonPlatoon* msg);
    void handleNewFormationMessage(NewFormation* msg);
    void sendNewFormationMessage(const std::vector<int>& newPlatoonFormation);
};

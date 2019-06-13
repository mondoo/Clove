namespace clv{
	template<typename T>
	inline void Keyboard::trimBuffer(std::queue<T>& buffer){
		while(buffer.size() > bufferSize){
			buffer.pop();
		}
	}
}
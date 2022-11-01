#pragma once

constexpr int kDefaultArraySize = 4;

template<typename T>

class Array final {

public:
	class Iterator final {

	};
	class ConstIterator final {

	};

	Array(int capacity) :
		size_(0){
		if (capacity < kDefaultArraySize) {
			capacity_ = kDefaultArraySize;
		}
		else {
			capacity_ = capacity;
		}
		pitems_ = (T*)malloc(sizeof(T) * capacity_);
	}
	Array() :
		Array(kDefaultArraySize)
	{}

	~Array() {
		for (int i = 0; i < size_; i++) {
			pitems_[i].~T();
		}
		free(pitems_);
	}

	Array(const Array& other) {
		capacity_ = other.capacity_;
		size_ = other.size_;
		pitems_ = malloc(sizeof(T) * capacity_);
		for (int i = 0; i < size_; i++) {
			new (pitems_ + i) T(other.pitems_[i]);
		}
	}

	Array(Array&& other) {
		pitems_ = other.pitems_;
		size_ = other.capacity_;
		capacity_ = other.capacity_;
		other.pitems_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	Array& operator=(Array other) {
		swap(other);
		return *this;
	}

	Array& operator=(Array&& other) {
		for (int i = 0; i < size_; i++) {
			pitems_[i].~T();
		}
		free(pitems_);
		pitems_ = other.pitems_;
		size_ = other.capacity_;
		capacity_ = other.capacity_;
		other.pitems_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}
	
	//std::unique_ptr<Array>(const std::unique_ptr<Array> other) = delete;
	//std::unique_ptr operator=(const std::unique_ptr other) = delete;

	int size() const {
		return size_;
	}

	int insert(const T& value) {
		if (size_ >= capacity_) {
			capacity_ *= 2;
			T* p = (T*)malloc(capacity_ * sizeof(T));
			for (int i = 0; i < size_; i++) {
				new (p + i) T(std::move(pitems_[i]));
			}

			for (int i = 0; i < size_; i++) {
				pitems_[i].~T();
			}
			free(pitems_);
			pitems_ = p;
		}
		new (pitems_ + size_) T(value);
		size_ += 1;
		return size_ - 1;
	}

	int insert(int index, const T& value) {
		// Так как в задании сказано "увеличивать размер на 1" значет index <= size_, иначе я ввозвращаю -1, как ошибку вставки
		if (index > size_) {
			return -1;
		}
		if (size_ >= capacity_) {
			capacity_ *= 2;
			T* p = (T*)malloc(capacity_ * sizeof(T));
			

			for (int i = 0; i < index; i++) {
				new (p + i) T(std::move(pitems_[i]));
			}
			new (p + index) T(value);
			for (int i = index + 1; i < size_ + 1; i++) {
				new (p + i) T(std::move(pitems_[i - 1]));
			}

			for (int i = 0; i < size_; i++) {
				pitems_[i].~T();
			}
			size_ += 1;
			free(pitems_);
			pitems_ = p;
			return index;
		}
		//TODO
		return -1;
	}

	void remove(int index);

	const T& operator[](int index) const;
	T& operator[](int index);

	void swap(Array& other) {
		std::swap(pitems_, other.pitems_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	

	Iterator iterator();
	ConstIterator iterator() const;

	Iterator reverseIterator();
	ConstIterator reverseIterator() const;

private:
	int size_;
	int capacity_;
	T* pitems_;
};
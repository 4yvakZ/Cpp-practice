#pragma once
#include <cassert>

constexpr int kDefaultArraySize = 4;

template<typename T>

class Array final {

public:
	class Iterator final {
	public:
		friend Array;
		const T& get() const {
			return *pointer_;
		}
		void set(const T& value) {
			pointer_.~T();
			new (pointer_) T(value);
		}
		void next() {
			pointer_ += direction_;
		}
		bool hasNext() {
			return (pointer_ + direction_ >= array_->pitems_ 
				&& pointer_ + direction_ <= array_->pitems_ + array_->size_ - 1);
		}
	private:
		int direction_;
		T* pointer_;
		Array* array_;
	};
	class ConstIterator final {
	public:
		friend Array;
		const T& get() const {
			return *pointer_;
		}
		void next() {
			pointer_ += direction_;
		}
		bool hasNext() {
			return (pointer_ + direction_ >= array_->pitems_ 
				&& pointer_ + direction_ <= array_->pitems_ + array_->size_ - 1);
		}
	private:
		int direction_;
		T* pointer_;
		Array* array_;
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
	
	//std::unique_ptr<Array<T>>(const std::unique_ptr<Array<T>> other) = delete;
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
		if (index > size_ || index < 0) {
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
		size_ += 1;
		for (int i = size_ - 1; i > index; i--) {
			new (pitems_ + i) T(std::move(pitems_[i - 1]));
		}
		new (pitems_ + index) T(value);
		return index;
	}

	void remove(int index) {
		if (index >= size_ || index < 0) {
			return;
		}
		pitems_[index].~T();
		for (int i = index; i < size_ - 1; i++) {
			new (pitems_ + i) T(std::move(pitems_[i + 1]));
		}
		size_ -= 1;
	}

	const T& operator[](int index) const {
		assert(index >= 0 && index < size_);
		return *(pitems_ + index);
	}

	T& operator[](int index) {
		assert(index >= 0 && index < size_);
		return *(pitems_ + index);
	}

	void swap(Array& other) {
		std::swap(pitems_, other.pitems_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	Iterator iterator() {
		Iterator iterator;
		iterator.direction_ = 1;
		iterator.pointer_ = pitems_;
		iterator.array_ = this;
		return iterator;
	}
	ConstIterator iterator() const {
		ConstIterator constIterator;
		constIterator.direction_ = 1;
		constIterator.pointer_ = pitems_;
		constIterator.array_ = this;
		return constIterator;
	}

	Iterator reverseIterator() {
		Iterator iterator;
		iterator.direction_ = -1;
		iterator.pointer_ = pitems_ + size_ - 1;
		iterator.array_ = this;
		return iterator;
	}
	ConstIterator reverseIterator() const {
		ConstIterator constIterator;
		constIterator.direction_ = -1;
		constIterator.pointer_ = pitems_ + size_ - 1;
		constIterator.array_ = this;
		return constIterator;
	}

private:
	int size_;
	int capacity_;
	T* pitems_;
};
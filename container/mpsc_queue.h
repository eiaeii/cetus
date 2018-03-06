#pragma once

#include <atomic>

namespace terra
{
	/**
	* Enumerates concurrent queue modes.
	*/
	enum class EQueueMode {
		/** Multiple-producers, single-consumer queue. */
		Mpsc,

		/** Single-producer, single-consumer queue. */
		Spsc
	};

	/**
	* Template for queues.
	*
	* This template implements an unbounded non-intrusive queue using a lock-free linked
	* list that stores copies of the queued items. The template can operate in two modes:
	* Multiple-producers single-consumer (MPSC) and Single-producer single-consumer (SPSC).
	*
	* The queue is thread-safe in both modes. The Dequeue() method ensures thread-safety by
	* writing it in a way that does not depend on possible instruction reordering on the CPU.
	* The Enqueue() method uses an atomic compare-and-swap in multiple-producers scenarios.
	*
	* @param ItemType The type of items stored in the queue.
	* @param Mode The queue mode (single-producer, single-consumer by default).
	* @todo gmp: Implement node pooling.
	*/
	template <typename ItemType, EQueueMode Mode = EQueueMode::Spsc>
	class TQueue
	{
	public:
		/** Default constructor. */
		TQueue() : head_(new TNode()), tail_(head_.load()) {}

		/** Destructor. */
		~TQueue()
		{
			while (TNode* const node = tail_.load()) {
				tail_.store(node->next_node);
				delete node;
			}
		}

	public:
		/**
		* Removes and returns the item from the tail of the queue.
		*
		* @param OutValue Will hold the returned value.
		* @return true if a value was returned, false if the queue was empty.
		* @see Enqueue, IsEmpty, Peek
		*/
		bool Dequeue(ItemType& out_item)
		{
			TNode* tail = tail_.load();
			TNode* popped = tail->next_node;

			if (popped == nullptr)
			{
				return false;
			}

			out_item = std::move(popped->item_data);

			TNode* old_tail = tail;
			popped->item_data = ItemType();
			tail_.store(popped);
			delete old_tail;

			return true;
		}

		/** Empty the queue, discarding all items. */
		void Empty()
		{
			ItemType dummy_item;
			while (Dequeue(dummy_item));
		}

		/**
		* Adds an item to the head of the queue.
		*
		* @param Item The item to add.
		* @return true if the item was added, false otherwise.
		* @see Dequeue, IsEmpty, Peek
		*/
		bool Enqueue(const ItemType& item_data)
		{
			TNode* new_node = new TNode(item_data);
			TNode* old_head;

			if (Mode == EQueueMode::Mpsc)
			{
				old_head = std::atomic_exchange(&head_, new_node);
			}
			else
			{
				old_head = head_.load();
				head_.store(new_node);
			}

			old_head->next_node = new_node;

			return true;
		}

		/**
		* Adds an item to the head of the queue.
		*
		* @param Item The item to add.
		* @return true if the item was added, false otherwise.
		* @see Dequeue, IsEmpty, Peek
		*/
		bool Enqueue(ItemType&& item_data)
		{
			TNode* new_node = new TNode(std::move(item_data));
			TNode* old_head;
			if (Mode == EQueueMode::Mpsc)
			{
				old_head = std::atomic_exchange(&head_, new_node);
			}
			else
			{
				old_head = head_.load();
				head_.store(new_node);
			}

			old_head->next_node = new_node;
			return true;
		}

		/**
		* Checks whether the queue is empty.
		*
		* @return true if the queue is empty, false otherwise.
		* @see Dequeue, Enqueue, Peek
		*/
		bool IsEmpty() const { return (tail_.load()->next_node == nullptr); }

		/**
		* Peeks at the queue's tail item without removing it.
		*
		* @param OutItem Will hold the peeked at item.
		* @return true if an item was returned, false if the queue was empty.
		* @see Dequeue, Enqueue, IsEmpty
		*/
		bool Peek(ItemType& out_item) const
		{
			TNode* tail = tail_.load();
			if (tail->next_node == nullptr) {
				return false;
			}
			out_item = tail->next_node->item_data;
			return true;
		}

	private:
		/** Structure for the internal linked list. */
		struct TNode {
			/** Holds a pointer to the next node in the list. */
			TNode* next_node;

			/** Holds the node's item. */
			ItemType item_data;

			/** Default constructor. */
			TNode() : next_node(nullptr), item_data() {}

			/** Creates and initializes a new node. */
			explicit TNode(const ItemType& item) : next_node(nullptr), item_data(item) {}

			/** Creates and initializes a new node. */
			explicit TNode(ItemType&& item) : next_node(nullptr), item_data(std::move(item)) {}
		};

		/** Holds a pointer to the head of the list. */
		std::atomic<TNode*> head_;

		/** Holds a pointer to the tail of the list. */
		std::atomic<TNode*> tail_;

	private:
		/** Hidden copy constructor. */
		TQueue(const TQueue&) = delete;

		/** Hidden assignment operator. */
		TQueue& operator=(const TQueue&) = delete;
	};
}
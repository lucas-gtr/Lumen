#include <gtest/gtest.h>
#include "Core/Observer.hpp"
#include <string>

TEST(ObserverTest, AddAndNotifySingleCallback) {
    Observer<int> observer;
    int received_value = 0;
    observer.add([&](int value) { received_value = value; });
    observer.notify(42);
    EXPECT_EQ(received_value, 42);
}

TEST(ObserverTest, AddAndNotifyMultipleCallbacks) {
    Observer<std::string> observer;
    std::string first_receiver;
    std::string second_receiver;
    observer.add([&](const std::string& value) { first_receiver = value; });
    observer.add([&](const std::string& value) { second_receiver = value; });
    observer.notify("hello");
    EXPECT_EQ(first_receiver, "hello");
    EXPECT_EQ(second_receiver, "hello");
}

TEST(ObserverTest, RemoveCallbackPreventsNotification) {
    Observer<> observer;
    bool was_called = false;
    auto callback_id = observer.add([&]() { was_called = true; });
    observer.remove(callback_id);
    observer.notify();
    EXPECT_FALSE(was_called);
}

TEST(ObserverTest, ClearResetsCallbacksAndIds) {
    Observer<int> observer;

    auto first_id  = observer.add([](int) {});
    auto second_id = observer.add([](int) {});
    EXPECT_EQ(first_id, 0u);
    EXPECT_EQ(second_id, 1u);

    observer.clear();

    int call_count_after_clear = 0;
    auto new_id = observer.add([&](int) { ++call_count_after_clear; });
    observer.notify(7);

    EXPECT_EQ(new_id, 0u);                 // compteur ID remis à zéro
    EXPECT_EQ(call_count_after_clear, 1);  // seul le nouveau callback est appelé
}

TEST(ObserverTest, EmptyCallbackIsIgnored) {
    Observer<int> observer;
    observer.add(std::function<void(int)>{});  // callback vide
    EXPECT_NO_THROW(observer.notify(5));       // ne lance pas d'exception
}

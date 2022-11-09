#include <gtest/gtest.h>
#include <memory>
#include <exception>

#include "UniqueID.h"
#include "unique_id_builder.h"

TEST(UniqueID, UniqueIDHasValue) {
    UniqueID uniqueID;
    EXPECT_TRUE(uniqueID.getValue().length() > 0);
}

TEST(UniqueIDBuilder, UniqueIDBuilderCreatesUniqueID) {
    UniqueIDBuilder uniqueIDBuilder;
    EXPECT_TRUE(uniqueIDBuilder.createUniqueID().getValue().length() > 0);
}
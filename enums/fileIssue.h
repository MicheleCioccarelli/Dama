#pragma once

enum fileIssue {
    FILE_CLOSED_CORRECTELY,
    // The file path specified was not found
    INVALID_PATH,
    INVALID_FILE_NAME,
    // There was an unknown issue opening the file (probably wrong path)
    OPENING_ISSUE,
    // There was a problem creating and accessing the file
    CREATING_ISSUE
};
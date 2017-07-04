#include <iostream>
#include <string>

#include <cassandra.h>
#include <stdio.h>
#include <unistd.h>


int send_select(int k){
    /* Setup and connect to cluster */
    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    const char* hosts = "127.0.0.1";
    /*if (argc > 1) {
        hosts = argv[1];
    }*/

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, hosts);

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect(session, cluster);

    if (cass_future_error_code(connect_future) == CASS_OK) {
        CassFuture* close_future = NULL;

        /* Build statement and execute query */
        //const char* query = "SELECT release_version FROM system.local";
        const char* query = "SELECT user_name, state, birth_year FROM test_keyspace.users";
        CassStatement* statement = cass_statement_new(query, 0);

        CassFuture* result_future = cass_session_execute(session, statement);

        if (cass_future_error_code(result_future) == CASS_OK) {
            // Retrieve result set and get the first row
            const CassResult* result = cass_future_get_result(result_future);
            const CassRow* row = cass_result_first_row(result);

            if (row) {
                //const CassValue* value = cass_row_get_column_by_name(row, "release_version");
                const CassValue* value = cass_row_get_column_by_name(row, "user_name");

                const char* user_name;
                size_t user_name_length;
                cass_value_get_string(value, &user_name, &user_name_length);
                printf("user_name: '%.*s'\n", (int)user_name_length, user_name);
            }

            cass_result_free(result);
        } else {
            // Handle error
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length, message);
        }

        //cass_statement_free(statement);
        //cass_future_free(result_future);

        /* Close the session */
        close_future = cass_session_close(session);
        cass_future_wait(close_future);
        cass_future_free(close_future);
    } else {
        /* Handle error */
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);
        fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
    }

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
}

int send_insert(int k) {
    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    const char* hosts = "127.0.0.1";
    /*if (argc > 1) {
        hosts = argv[1];
    }*/

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, hosts);

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect(session, cluster);

    std::string query = "insert into test_keyspace.users (user_name, state, birth_year) values ('a" +
            std::to_string(k) +
            "', 'man', 1942)";
    //CassString insert_query = cass_string_init("INSERT INTO example (key, value) VALUES (?, ?);");

    /* There are two bind variables in the query string */
    CassStatement* statement = cass_statement_new(query.c_str(), 0);

    /* Bind the values using the indices of the bind variables */
    //cass_statement_bind_string(statement, 0, "abc");
    //cass_statement_bind_int32(statement, 1, 123);

    CassFuture* query_future = cass_session_execute(session, statement);

    /* Statement objects can be freed immediately after being executed */
    cass_statement_free(statement);

    /* This will block until the query has finished */
    CassError rc = cass_future_error_code(query_future);

    printf("Query result: %s\n", cass_error_desc(rc));

    cass_future_free(query_future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return 0;
}


int main(int argc, char* argv[]) {
    int count_msgs = 1;
    int count_thrs = 1;
    time_t timer = time(0);
    tm *ltm = localtime(&timer);
    std::cerr << ltm->tm_min << ":" << ltm->tm_sec << "\n";

    for (int k = 0; k < count_msgs; k++) {
        send_insert(k);
    }

    time_t timer2 = time(0);
    tm *ltm2 = localtime(&timer2);
    std::cerr << ltm2->tm_min << ":" << ltm2->tm_sec << "\n";

    time_t timer3 = timer2 - timer;
    tm *ltm3 = localtime(&timer3);
    std::cerr << ltm3->tm_min << ":" << ltm3->tm_sec << "\n";
    int time_sec = (ltm3->tm_sec + ltm3->tm_min * 60);
    if (count_msgs) {
        int time_sec = (ltm3->tm_sec + ltm3->tm_min * 60);
        if (time_sec) {
            std::cerr << count_msgs/time_sec << "\n";
            std::cerr << count_msgs/time_sec/count_thrs << "\n";
        }
    }

    return 0;
}

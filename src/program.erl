-module(program).
-behaviour(gen_server).

%% API.
-export([start_link/2]).

%% gen_server.
-export([init/1]).
-export([handle_call/3]).
-export([handle_cast/2]).
-export([handle_info/2]).
-export([terminate/2]).
-export([code_change/3]).
-export([stop/1]).

-record(state, {port, options = []}).

%% API.

-spec(start_link/2::(atom(), [{atom(), any()}]) -> {ok, pid()}).
start_link(Name, Options) ->
  gen_server:start_link({local, Name}, ?MODULE, Options, []).

%% gen_server.

-spec(init/1::([any()]) -> {ok, pid()}).
init(Options) ->
  Program = proplists:get_value(program, Options),
  Port = open_port({spawn, Program}, [{packet, 2}, binary]),
  {ok, #state{port = Port, options = Options}}.

stop(Name) -> gen_server:call(Name, stop).

%%%%%% GEN_SERVER HANDLERS %%%%%%%%%%%%%
handle_call(stop, _From, State) -> {stop, normal, ok, State};
handle_call(_Request, _From, State) -> {reply, ignored, State}.

handle_cast(_Msg, State) -> {noreply, State}.

handle_info(_Info, State) -> {noreply, State}.

terminate(normal, #state{port = Port}) ->
  exec(Port, {stop, nop}),
  port_close(Port),
  ok;
terminate(_Reason, _State) -> ok.

code_change(_OldVsn, State, _Extra) -> {ok, State}.


%%%%% PORT FUNCTIONS %%%%%%%%%%
exec(Port, Cmd) ->
  port_command(Port, term_to_binary(Cmd)),
  receive
    {Port, {data, Data}} when is_binary(Data) ->
      binary_to_term(Data);
    _ ->
      ok
  end.

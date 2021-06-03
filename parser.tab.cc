// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 23 "parser.yy"

#include "driver.h"
#include "flags.h"
#include "semantic.cpp"
#include <iostream>
#include <vector>
#include <stack>

Scope scope = Scope();
int tempCount = 0;
ExpressionSolver es;
stack<ParamSolver> params;

stack<ExpressionSolver> pes;
Quad quads = Quad();
VarTable::symbolRow * var = NULL;

#line 63 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 6 "parser.yy"
namespace yy {
#line 155 "parser.tab.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (parser_driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 51: // CHAR
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
        break;

      case 52: // INT
      case 53: // FLOAT
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case 49: // ID
      case 50: // STRING
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 51: // CHAR
        value.move< char > (YY_MOVE (that.value));
        break;

      case 52: // INT
      case 53: // FLOAT
        value.move< float > (YY_MOVE (that.value));
        break;

      case 49: // ID
      case 50: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 51: // CHAR
        value.copy< char > (that.value);
        break;

      case 52: // INT
      case 53: // FLOAT
        value.copy< float > (that.value);
        break;

      case 49: // ID
      case 50: // STRING
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 51: // CHAR
        value.move< char > (that.value);
        break;

      case 52: // INT
      case 53: // FLOAT
        value.move< float > (that.value);
        break;

      case 49: // ID
      case 50: // STRING
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
      case 49: // ID
#line 131 "parser.yy"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 407 "parser.tab.cc"
        break;

      case 50: // STRING
#line 131 "parser.yy"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 413 "parser.tab.cc"
        break;

      case 51: // CHAR
#line 131 "parser.yy"
                 { yyoutput << yysym.value.template as < char > (); }
#line 419 "parser.tab.cc"
        break;

      case 52: // INT
#line 131 "parser.yy"
                 { yyoutput << yysym.value.template as < float > (); }
#line 425 "parser.tab.cc"
        break;

      case 53: // FLOAT
#line 131 "parser.yy"
                 { yyoutput << yysym.value.template as < float > (); }
#line 431 "parser.tab.cc"
        break;

      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 51: // CHAR
        yylhs.value.emplace< char > ();
        break;

      case 52: // INT
      case 53: // FLOAT
        yylhs.value.emplace< float > ();
        break;

      case 49: // ID
      case 50: // STRING
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 4:
#line 142 "parser.yy"
                      {
                quads.prepareMainQuad();
        }
#line 688 "parser.tab.cc"
    break;

  case 5:
#line 144 "parser.yy"
                      {
                scope.freeClassScope();
                classFlag = 0;
                globalFlag = 1;
        }
#line 698 "parser.tab.cc"
    break;

  case 6:
#line 148 "parser.yy"
                    {
                globalFlag = 0;
                onClass = 0;
        }
#line 707 "parser.tab.cc"
    break;

  case 8:
#line 155 "parser.yy"
                 {setID(yystack_[0].value.as < std::string > (),0,0);}
#line 713 "parser.tab.cc"
    break;

  case 10:
#line 156 "parser.yy"
                          {setID(yystack_[0].value.as < std::string > (),0,0);}
#line 719 "parser.tab.cc"
    break;

  case 17:
#line 162 "parser.yy"
                          {setID(yystack_[0].value.as < std::string > (),0,0);}
#line 725 "parser.tab.cc"
    break;

  case 19:
#line 163 "parser.yy"
                          {setID(yystack_[0].value.as < std::string > (),0,0);}
#line 731 "parser.tab.cc"
    break;

  case 21:
#line 168 "parser.yy"
                          {currentIds.back().dim1 = yystack_[0].value.as < float > ();}
#line 737 "parser.tab.cc"
    break;

  case 23:
#line 169 "parser.yy"
                      {currentIds.back().dim2 = yystack_[0].value.as < float > ();}
#line 743 "parser.tab.cc"
    break;

  case 25:
#line 173 "parser.yy"
                  {
        int id = 0;
        if(paramsFlag){
                id = pes.top().popOprnd();
        }else{
                if(expressionFlag){
                        id = es.popOprnd();
                }else{
                        if(onClass && !classFlag)
                                classAssign = 1;
                        id = var->dir;
                }
        }
        if(onClass)
                params.push(ParamSolver(id,1));
        else{
                if(classFlag)
                        params.push(ParamSolver(id,1));
                else
                        params.push(ParamSolver(id,0));
        }
        if(onClass && !classFlag){
                scope.freeClassScope();
                onClass = 0;
        }
        paramsFlag = 1;
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
}
#line 778 "parser.tab.cc"
    break;

  case 26:
#line 202 "parser.yy"
             {
        if(paramsFlag){
                quads.importSolver(pes.top());
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParamArray(single);
                        int dim = params.top().getDimension(paramNum);
                        quads.insertQuad(205,single,-1,dim);
                        if(params.top().getDimension(2) > 0){
                                int temp = params.top().generateArrayQuad();
                                int dimConstant = scope.addConstantINTVarDir(params.top().getDimension(2));
                                quads.insertQuad(2,single,dimConstant,temp);
                                matrixDir.push(temp);
                        }else{
                                int pointer = params.top().generatePointerQuad();
                                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                                quads.insertQuad(0,arrayDirConstant,single,pointer);
                        }
                }else{
                        int paramNum = params.top().insertParamArray(quads.getLastQuad().result);
                        int dim = params.top().getDimension(paramNum);
                        quads.insertQuad(205,quads.getLastQuad().result,-1,dim);
                        if(params.top().getDimension(2) > 0){
                                int temp = params.top().generateArrayQuad();
                                int dimConstant = scope.addConstantINTVarDir(params.top().getDimension(2));
                                quads.insertQuad(2,quads.getLastQuad().result,dimConstant,temp);
                                matrixDir.push(temp);
                        }else{
                                int pointer = params.top().generatePointerQuad();
                                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                                quads.insertQuad(0,arrayDirConstant,quads.getLastQuad().result,pointer);
                        }
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
}
#line 826 "parser.tab.cc"
    break;

  case 27:
#line 244 "parser.yy"
              {
        
        params.top().checkParams();
        if(params.size()<2){
                paramsFlag = 0; 
        }
        int pointer = quads.getLastQuad().result;
        
        if(params.size()>1){
                if(params.top().isClass() && !classFlag){
                        int temp = scope.insertVarOnScope("_"+to_string(tempCount++),params.top().getArrayType());
                        quads.insertQuad(5,quads.getLastQuad().result,-1,temp);
                        quads.insertQuad(207,-1,-1,-1);
                        pointer = temp;
                }
                pes.top().insertOprndDir(pointer);
        }else{
                if(expressionFlag){  
                        if(params.top().isClass() && !classFlag){
                                int temp = scope.insertVarOnScope("_"+to_string(tempCount++),params.top().getArrayType());
                                quads.insertQuad(5,quads.getLastQuad().result,-1,temp);
                                quads.insertQuad(207,-1,-1,-1);
                                pointer = temp;
                        }
                        es.insertOprndDir(pointer);
                }else{
                        var = scope.getGlobalVariable(pointer);
                }
        }

        params.pop();
}
#line 863 "parser.tab.cc"
    break;

  case 28:
#line 276 "parser.yy"
              {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
}
#line 873 "parser.tab.cc"
    break;

  case 29:
#line 280 "parser.yy"
             {
        if(paramsFlag){
                int result = matrixDir.top();
                quads.importSolver(pes.top()); 
                int single = pes.top().singleVar();
                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                if(single){
                        int paramNum = params.top().insertParamArray(single);
                        int dim = params.top().getDimension(paramNum);
                        int temp = params.top().generateArrayQuad();
                        int pointer = params.top().generatePointerQuad();
                        quads.insertQuad(205,single,-1,dim);
                        quads.insertQuad(0,result,single,temp);
                        quads.insertQuad(0,arrayDirConstant,temp,pointer);
                }else{
                        int paramNum = params.top().insertParamArray(quads.getLastQuad().result);
                        int dim = params.top().getDimension(paramNum);
                        int temp = params.top().generateArrayQuad();
                        int pointer = params.top().generatePointerQuad();
                        quads.insertQuad(205,quads.getLastQuad().result,-1,dim);
                        quads.insertQuad(0,result,quads.getLastQuad().result,temp);
                        quads.insertQuad(0,arrayDirConstant,temp,pointer);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
}
#line 911 "parser.tab.cc"
    break;

  case 31:
#line 318 "parser.yy"
                     {
        if(paramsFlag){
                pes.top().installPar();
        }else{
                if(expressionFlag){
                        es.installPar();
                }
        }
        }
#line 925 "parser.tab.cc"
    break;

  case 32:
#line 326 "parser.yy"
                          {
                if(paramsFlag){
                        pes.top().closePar();
                }else{
                        if(expressionFlag){
                                es.closePar();
                        }
                }
        }
#line 939 "parser.tab.cc"
    break;

  case 33:
#line 337 "parser.yy"
                    {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
}
#line 949 "parser.tab.cc"
    break;

  case 34:
#line 341 "parser.yy"
             {
        if(paramsFlag){
                quads.importSolver(pes.top()); 
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParam(single);
                        quads.insertQuad(203,single,-1,paramNum);
                }else{
                        int paramNum = params.top().insertParam(quads.getLastQuad().result);
                        quads.insertQuad(203,quads.getLastQuad().result,-1,paramNum);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
}
#line 975 "parser.tab.cc"
    break;

  case 37:
#line 364 "parser.yy"
                       {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
}
#line 985 "parser.tab.cc"
    break;

  case 38:
#line 368 "parser.yy"
             {
        if(paramsFlag){
                quads.importSolver(pes.top());
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParam(single);
                        quads.insertQuad(203,single,-1,paramNum);
                }else{
                        int paramNum = params.top().insertParam(quads.getLastQuad().result);
                        quads.insertQuad(203,quads.getLastQuad().result,-1,paramNum);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
}
#line 1011 "parser.tab.cc"
    break;

  case 41:
#line 392 "parser.yy"
                   {
        scope.resetClassConts();
        classFlag = 1;
        setClassName(yystack_[0].value.as < std::string > ());
        scope.globalInsertClass(yystack_[0].value.as < std::string > (),10,quads.getQuadLastDir());
        scope.setClassScope(yystack_[0].value.as < std::string > ());
}
#line 1023 "parser.tab.cc"
    break;

  case 44:
#line 402 "parser.yy"
                                   {
        scope.resetLocalConts();
        if(classFlag){
                scope.insertClassFunction(yystack_[0].value.as < std::string > (),currentType,quads.getQuadLastDir());
                scope.setClassFunctionScope(yystack_[0].value.as < std::string > ());
                if(currentType != 20){
                        string varName = "_" + getClassName() + yystack_[0].value.as < std::string > ();
                        scope.insertGlobalVariable(varName,currentType);
                }
        }else{
                scope.globalInsertFunction(yystack_[0].value.as < std::string > (),currentType,quads.getQuadLastDir());
                scope.setFunctionScope(yystack_[0].value.as < std::string > ());
                if(currentType != 20){ 
                        scope.insertGlobalVariable(yystack_[0].value.as < std::string > (),currentType);
                }
        }
        resetCurrentType();
}
#line 1046 "parser.tab.cc"
    break;

  case 45:
#line 419 "parser.yy"
                                                         {
        quads.insertQuad(299,-1,-1,-1);
}
#line 1054 "parser.tab.cc"
    break;

  case 48:
#line 425 "parser.yy"
            {
        scope.resetLocalConts();
        quads.setMainQuad();
        scope.setFunctionScope("global");
}
#line 1064 "parser.tab.cc"
    break;

  case 49:
#line 429 "parser.yy"
                               {quads.insertQuad(999,-1,-1,-1);}
#line 1070 "parser.tab.cc"
    break;

  case 56:
#line 446 "parser.yy"
{       
        
        if(globalFlag){
                for(auto var : currentIds){
                        scope.insertGlobalVariable(var.id, var.dim1,var.dim2,currentType);
                }
        }else{
                for(auto var : currentIds){
                        scope.insertVarOnScope(var.id, var.dim1,var.dim2,currentType);
                } 
        }
        
        resetID();
        resetCurrentType();
}
#line 1090 "parser.tab.cc"
    break;

  case 62:
#line 473 "parser.yy"
                                {
        if(classFlag){
                for(auto var : currentIds){
                        scope.insertVarOnClassScope(var.id, var.dim1,var.dim2,currentType);
                }
        }
        resetID();
        resetCurrentType();
}
#line 1104 "parser.tab.cc"
    break;

  case 66:
#line 488 "parser.yy"
                       {setCurrentType(0);}
#line 1110 "parser.tab.cc"
    break;

  case 67:
#line 489 "parser.yy"
                          {setCurrentType(1);}
#line 1116 "parser.tab.cc"
    break;

  case 68:
#line 490 "parser.yy"
                         {setCurrentType(2);}
#line 1122 "parser.tab.cc"
    break;

  case 70:
#line 493 "parser.yy"
                               {setCurrentType(scope.getClassType(yystack_[0].value.as < std::string > ()));}
#line 1128 "parser.tab.cc"
    break;

  case 72:
#line 496 "parser.yy"
                                    {setCurrentType(20);}
#line 1134 "parser.tab.cc"
    break;

  case 73:
#line 499 "parser.yy"
               {
        VarTable::symbolRow * variable = NULL;
        if(!onClass)
                variable = scope.getGlobalVariable(yystack_[0].value.as < std::string > ());
        else
                variable = scope.getClassVariable(yystack_[0].value.as < std::string > ());
        if(variable && variable->type >= 100){
                scope.setClassScopeID(variable->type);
                quads.insertQuad(206,variable->type,variable->dir,-1);
                onClass = 1;
        }else{
                if((onClass ? scope.getClassFunction(yystack_[0].value.as < std::string > ()) : scope.globalGetFunction(yystack_[0].value.as < std::string > ()))){
                        if(onClass){
                                params.push(ParamSolver(yystack_[0].value.as < std::string > (),1));
                                onClass = 0;
                        }else{
                                params.push(ParamSolver(yystack_[0].value.as < std::string > (),0));
                        }
                        paramsFlag = 1;
                        quads.insertQuad(202,params.top().getFunctionQuadDir(),-1,-1);    
                }else{
                        if(paramsFlag){
                                pes.top().insertOprnd(yystack_[0].value.as < std::string > ());
                                if(onClass){
                                        int dir = pes.top().popOprnd();
                                        if(variable->dimension1 > 0){
                                                pes.top().insertOprnd(yystack_[0].value.as < std::string > ());
                                        }else{
                                                int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++),var->type);
                                                quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                quads.insertQuad(5,dir,-1,dir2);
                                                pes.top().insertOprndDir(dir2);
                                                pes.top().insertClassInstance();
                                                onClass = 0;
                                                scope.freeClassScope();
                                        }
                                }
                        }else{
                                if(expressionFlag){
                                        es.insertOprnd(yystack_[0].value.as < std::string > ());
                                        if(onClass){
                                                int dir = es.popOprnd();
                                                if(variable->dimension1 > 0){
                                                        es.insertOprnd(yystack_[0].value.as < std::string > ());
                                                }else{
                                                        int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++)+var->name,var->type);
                                                        quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                        quads.insertQuad(5,dir,-1,dir2);
                                                        es.insertOprndDir(dir2);
                                                        onClass = 0;
                                                        es.insertClassInstance();
                                                        scope.freeClassScope();
                                                }
                                        }
                                }else{
                                        var = scope.getVarOnScope(yystack_[0].value.as < std::string > ());
                                        if(var == NULL && scope.classScope)
                                                var = scope.getClassVariable(yystack_[0].value.as < std::string > ());
                                        if(var == NULL)
                                                var = scope.getGlobalVariable(yystack_[0].value.as < std::string > ());
                                        if(onClass){
                                                int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++),var->type);
                                                quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                classAssign = 1;
                                                if(var->dimension1 <= 0)
                                                        quads.insertQuad(5,var->dir,-1,dir2);
                                        }
                                }
                        }
                }
        }
}
#line 1211 "parser.tab.cc"
    break;

  case 75:
#line 572 "parser.yy"
                     {if(paramsFlag && params.size()<1)throw CompilerError("Error: No parameters inserted");}
#line 1217 "parser.tab.cc"
    break;

  case 78:
#line 574 "parser.yy"
                       {
                params.top().checkParams();
                if(params.size()<2){
                        paramsFlag = 0; 
                } 
                quads.insertQuad(204,params.top().getFunctionGlobalDir(),-1,-1);
                if(params.top().getFunctionGlobalDir() != -1){
                        int dir = params.top().generateReturnQuad();
                        quads.insertQuad(5,params.top().getFunctionGlobalDir(),-1,dir);
                        if(params.size()>1){
                                pes.top().insertOprndDir(dir);
                        }else{
                                if(expressionFlag){
                                        es.insertOprndDir(dir);
                                }
                        }
                }
                if(params.top().isClass()){
                        quads.insertQuad(207,-1,-1,-1);
                }
                params.pop();
                scope.freeClassScope();
        }
#line 1245 "parser.tab.cc"
    break;

  case 80:
#line 602 "parser.yy"
                {setID(yystack_[0].value.as < std::string > (),0,0);}
#line 1251 "parser.tab.cc"
    break;

  case 81:
#line 603 "parser.yy"
{
        scope.insertParameterOnScope(yystack_[4].value.as < std::string > (),currentType);
        resetCurrentType();
        resetID();
}
#line 1261 "parser.tab.cc"
    break;

  case 96:
#line 629 "parser.yy"
{
        es = ExpressionSolver();
        expressionFlag = 1;
        es.insertOprnd(var->name);
        es.insertOptr(5);
        if(onClass){
                setClassName(scope.freeClassScope());
                onClass = 0;
        }
}
#line 1276 "parser.tab.cc"
    break;

  case 97:
#line 640 "parser.yy"
{
        es.checkStacks();
        if(!classFlag && classAssign){
                scope.setClassScope(getClassName());
                quads.importSolver(es);
                scope.freeClassScope();
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }else{
                quads.importSolver(es);
        }
        if(classAssign){
                quads.insertQuad(207,-1,-1,-1);
        }
        classAssign = 0;
        onClass = 0;
        expressionFlag = 0;
}
#line 1302 "parser.tab.cc"
    break;

  case 99:
#line 667 "parser.yy"
                          {
        es = ExpressionSolver();
        expressionFlag = 1;
}
#line 1311 "parser.tab.cc"
    break;

  case 100:
#line 670 "parser.yy"
                  {
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        if(single){
                es.checkVarType(single);
                int var = scope.getScopeDir();
                quads.insertQuad(298,single,-1,var);
        }else{
                es.checkVarType(quads.getLastQuad().result);
                int var = scope.getScopeDir();
                quads.insertQuad(298,quads.getLastQuad().result,-1,var);
        }
        if(!classFlag){
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }
}
#line 1337 "parser.tab.cc"
    break;

  case 101:
#line 693 "parser.yy"
                                     { 
        quads.insertQuad(103,-1,-1,var->dir);
        if(onClass){
                quads.insertQuad(207,-1,-1,-1);
                onClass = 0;
        }
}
#line 1349 "parser.tab.cc"
    break;

  case 102:
#line 702 "parser.yy"
                      { es = ExpressionSolver(); expressionFlag = 1;}
#line 1355 "parser.tab.cc"
    break;

  case 104:
#line 703 "parser.yy"
                              {
                        expressionFlag = 0;
                        int text = scope.addConstantSTRINGVarDir(yystack_[0].value.as < std::string > ());
                        quads.insertQuad(102,text,-1,-1);
                        int newLine = scope.addConstantSTRINGVarDir("\n");
                        quads.insertQuad(102,newLine,-1,-1);
                        }
#line 1367 "parser.tab.cc"
    break;

  case 105:
#line 710 "parser.yy"
                         {
                        expressionFlag = 0;
                        int text = scope.addConstantSTRINGVarDir(yystack_[0].value.as < std::string > ());
                        quads.insertQuad(102,text,-1,-1);
                        expressionFlag = 1;
                }
#line 1378 "parser.tab.cc"
    break;

  case 107:
#line 716 "parser.yy"
                             {
                        quads.importSolver(es); 
                        expressionFlag = 0;
                        int single = es.singleVar();
                        if(single){
                                quads.insertQuad(102,single,-1,-1);
                        }else{
                                quads.insertQuad(102,quads.getLastQuad().result,-1,-1);
                        }
                        if(!classFlag){
                                int inst = es.returnClassInstances();
                                while(inst > 0){
                                        inst--;
                                        quads.insertQuad(207,-1,-1,-1);
                                }
                        }
                        int newLine = scope.addConstantSTRINGVarDir("\n");
                        quads.insertQuad(102,newLine,-1,-1);
                }
#line 1402 "parser.tab.cc"
    break;

  case 108:
#line 735 "parser.yy"
                                  {
                        quads.importSolver(es); 
                        expressionFlag = 0;
                        int single = es.singleVar();
                        if(single){
                                quads.insertQuad(102,single,-1,-1);
                        }else{
                                quads.insertQuad(102,quads.getLastQuad().result,-1,-1);
                        }
                        if(!classFlag){
                                int inst = es.returnClassInstances();
                                while(inst > 0){
                                        inst--;
                                        quads.insertQuad(207,-1,-1,-1);
                                }
                        }
                        es = ExpressionSolver(); 
                        expressionFlag = 1;
                }
#line 1426 "parser.tab.cc"
    break;

  case 110:
#line 756 "parser.yy"
                             {paramsFlag = 0; es = ExpressionSolver(); expressionFlag = 1;}
#line 1432 "parser.tab.cc"
    break;

  case 111:
#line 756 "parser.yy"
                                                                                                            {
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        setJDir(quads.getQuadLastDir());
        if(single){
                quads.insertQuad(201,single,-1,-1);
        }else{
                quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
        }
}
#line 1448 "parser.tab.cc"
    break;

  case 113:
#line 768 "parser.yy"
                { 
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir()+1); 
                setJDir(quads.getQuadLastDir());
                quads.insertQuad(200,-1,-1,-1);
        }
#line 1459 "parser.tab.cc"
    break;

  case 114:
#line 773 "parser.yy"
                             {
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir());
        }
#line 1468 "parser.tab.cc"
    break;

  case 115:
#line 777 "parser.yy"
                {
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir()); 
        }
#line 1477 "parser.tab.cc"
    break;

  case 118:
#line 786 "parser.yy"
                           {es.checkStacks();}
#line 1483 "parser.tab.cc"
    break;

  case 121:
#line 788 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(6);}else{ if(expressionFlag){es.insertOptr(6);}}}
#line 1489 "parser.tab.cc"
    break;

  case 122:
#line 789 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(7);}else{ if(expressionFlag){es.insertOptr(7);}}}
#line 1495 "parser.tab.cc"
    break;

  case 123:
#line 790 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(10);}else{ if(expressionFlag){es.insertOptr(10);}}}
#line 1501 "parser.tab.cc"
    break;

  case 124:
#line 791 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(11);}else{ if(expressionFlag){es.insertOptr(11);}}}
#line 1507 "parser.tab.cc"
    break;

  case 125:
#line 792 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(8);}else{ if(expressionFlag){es.insertOptr(8);}}}
#line 1513 "parser.tab.cc"
    break;

  case 126:
#line 793 "parser.yy"
           {if(paramsFlag){pes.top().insertOptr(9);}else{ if(expressionFlag){es.insertOptr(9);}}}
#line 1519 "parser.tab.cc"
    break;

  case 127:
#line 794 "parser.yy"
                   {if(paramsFlag){pes.top().insertOptr(12);}else{ if(expressionFlag){es.insertOptr(12);}}}
#line 1525 "parser.tab.cc"
    break;

  case 128:
#line 794 "parser.yy"
                                                                                                                       {if(paramsFlag){pes.top().checkStacks();}else{ if(expressionFlag){es.checkStacks();}}}
#line 1531 "parser.tab.cc"
    break;

  case 129:
#line 795 "parser.yy"
                   {if(paramsFlag){pes.top().insertOptr(13);}else{ if(expressionFlag){es.insertOptr(13);}}}
#line 1537 "parser.tab.cc"
    break;

  case 130:
#line 795 "parser.yy"
                                                                                                                       {if(paramsFlag){pes.top().checkStacks();}else{ if(expressionFlag){es.checkStacks();}}}
#line 1543 "parser.tab.cc"
    break;

  case 132:
#line 799 "parser.yy"
           {
        if(paramsFlag){
                if(pes.top().operators.size() > 0){
                        if(pes.top().operators.top() == 0 || pes.top().operators.top() == 1){
                                pes.top().checkStacks();
                        }
                }
        }else{ 
                if(expressionFlag){
                        if(es.operators.size() > 0){
                                if(es.operators.top() == 0 || es.operators.top() == 1){
                                        es.checkStacks();
                                }
                        }
                }
        }
        }
#line 1565 "parser.tab.cc"
    break;

  case 134:
#line 816 "parser.yy"
               {if(paramsFlag){pes.top().insertOptr(0);}else{ if(expressionFlag){es.insertOptr(0);}}}
#line 1571 "parser.tab.cc"
    break;

  case 136:
#line 817 "parser.yy"
               {if(paramsFlag){pes.top().insertOptr(1);}else{ if(expressionFlag){es.insertOptr(1);}}}
#line 1577 "parser.tab.cc"
    break;

  case 139:
#line 821 "parser.yy"
              {
        if(paramsFlag){
                if(pes.top().operators.size() > 0){
                        if(pes.top().operators.top() == 2 || pes.top().operators.top() == 3 || pes.top().operators.top() == 4){
                                pes.top().checkStacks();
                        }
                }
        }else{
                if(expressionFlag){
                        if(es.operators.size() > 0){
                                if(es.operators.top() == 2 || es.operators.top() == 3 || es.operators.top() == 4){
                                        es.checkStacks();
                                }
                        }
                }
        }
        }
#line 1599 "parser.tab.cc"
    break;

  case 141:
#line 838 "parser.yy"
                {if(paramsFlag){pes.top().insertOptr(2);}else{if(expressionFlag){es.insertOptr(2);}}}
#line 1605 "parser.tab.cc"
    break;

  case 143:
#line 839 "parser.yy"
                {if(paramsFlag){pes.top().insertOptr(3);}else{if(expressionFlag){es.insertOptr(3);}}}
#line 1611 "parser.tab.cc"
    break;

  case 145:
#line 840 "parser.yy"
                {if(paramsFlag){pes.top().insertOptr(4);}else{if(expressionFlag){es.insertOptr(4);}}}
#line 1617 "parser.tab.cc"
    break;

  case 155:
#line 853 "parser.yy"
              {if(paramsFlag){pes.top().insertOprnd((int)yystack_[0].value.as < float > ());}else{ if(expressionFlag){es.insertOprnd((int)yystack_[0].value.as < float > ());}}}
#line 1623 "parser.tab.cc"
    break;

  case 156:
#line 854 "parser.yy"
                {if(paramsFlag){pes.top().insertOprnd((float)yystack_[0].value.as < float > ());}else{ if(expressionFlag){es.insertOprnd((float)yystack_[0].value.as < float > ());}}}
#line 1629 "parser.tab.cc"
    break;

  case 157:
#line 855 "parser.yy"
                {if(paramsFlag){pes.top().insertOprnd((char)yystack_[0].value.as < char > ());}else{ if(expressionFlag){es.insertOprnd((char)yystack_[0].value.as < char > ());}}}
#line 1635 "parser.tab.cc"
    break;

  case 158:
#line 858 "parser.yy"
                            {paramsFlag = 0; es = ExpressionSolver(); expressionFlag = 1;}
#line 1641 "parser.tab.cc"
    break;

  case 159:
#line 858 "parser.yy"
                                                                                                           {
        setJDir(quads.getQuadLastDir());
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        setJDir(quads.getQuadLastDir());
        if(!classFlag){
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }
        if(single){
                quads.insertQuad(201,single,-1,-1);
        }else{
                quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
        }
        
}
#line 1666 "parser.tab.cc"
    break;

  case 160:
#line 877 "parser.yy"
                          {
        int dir = getJDir();
        int exp = getJDir();
        quads.insertQuad(200,-1,-1,exp);
        quads.updateQuadResult(dir,quads.getQuadLastDir());
}
#line 1677 "parser.tab.cc"
    break;

  case 161:
#line 885 "parser.yy"
                                {
        if(onClass){
                quads.insertQuad(207,-1,-1,-1);
                onClass = 0;
        }
}
#line 1688 "parser.tab.cc"
    break;

  case 162:
#line 890 "parser.yy"
         {
        es = ExpressionSolver(); 
        paramsFlag = 0;
        expressionFlag = 1;
        setforID(var->name);
        es.insertOprnd(var->name);
        es.insertOptr(5);
}
#line 1701 "parser.tab.cc"
    break;

  case 163:
#line 897 "parser.yy"
      {
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0;
        string assign = var->name;
        es = ExpressionSolver(); 
        expressionFlag = 1;
        es.insertOprnd(assign);
}
#line 1715 "parser.tab.cc"
    break;

  case 164:
#line 905 "parser.yy"
     { es.insertOptr(11);}
#line 1721 "parser.tab.cc"
    break;

  case 165:
#line 905 "parser.yy"
                               {
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0; 
        int lastDir = quads.getQuadLastDir();
        setJDir(lastDir);
        quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
}
#line 1734 "parser.tab.cc"
    break;

  case 166:
#line 912 "parser.yy"
                          {
        string fid = getforID();
        es = ExpressionSolver(); 
        expressionFlag = 1;
        es.insertOprnd(fid);
        es.insertOptr(5);
        es.insertOprnd(fid);
        es.insertOptr(0);
        es.insertOprnd(1);
        es.checkStacks();
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0;

        int dir = getJDir();
        quads.insertQuad(200,-1,-1,dir-1);
        quads.updateQuadResult(dir,quads.getQuadLastDir());
}
#line 1757 "parser.tab.cc"
    break;


#line 1761 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -181;

  const signed char parser::yytable_ninf_ = -106;

  const short
  parser::yypact_[] =
  {
     -23,   -32,    20,  -181,  -181,  -181,    17,     9,   -17,  -181,
    -181,  -181,    12,    24,    32,  -181,  -181,    37,  -181,    56,
    -181,    78,  -181,    28,  -181,    64,    46,  -181,  -181,  -181,
    -181,  -181,  -181,    71,  -181,    75,    32,  -181,    79,    69,
     -11,    61,  -181,  -181,  -181,    74,    32,  -181,  -181,    81,
     118,  -181,    28,  -181,   116,  -181,  -181,  -181,  -181,  -181,
      82,    76,  -181,  -181,  -181,  -181,   121,  -181,  -181,  -181,
     128,    46,   130,   126,  -181,   124,   127,    32,     9,  -181,
      85,  -181,   129,    74,   131,   134,    93,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,    40,  -181,  -181,   132,  -181,
     135,   136,   137,   138,    94,  -181,  -181,    14,   142,    40,
      40,    40,    40,    40,    40,    40,  -181,  -181,   143,    12,
      15,    94,  -181,  -181,  -181,  -181,    31,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
     139,   147,  -181,  -181,  -181,  -181,  -181,   150,    77,  -181,
    -181,    47,   145,     4,    15,    15,   144,  -181,   146,  -181,
      31,  -181,  -181,    15,    15,   -11,    40,    15,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,    15,    26,    98,  -181,  -181,
    -181,  -181,  -181,  -181,   157,   160,   156,   167,   161,   162,
    -181,    94,   163,    15,  -181,   171,  -181,  -181,   164,   165,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -181,   174,   175,  -181,  -181,  -181,    15,  -181,  -181,  -181,
    -181,   177,   178,   179,  -181,    34,    15,    15,    15,    15,
      15,     4,  -181,     4,   140,   148,  -181,   181,  -181,  -181,
     169,    93,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,  -181,   176,   180,   151,  -181,
    -181,   182,    15,  -181,  -181,    28,    15,    15,    40,    40,
    -181,    15,  -181,  -181,  -181,  -181,  -181,   184,   185,    15,
    -181,   153,  -181,  -181,   181,  -181,  -181,  -181,   152,  -181,
     191,   193,    40,    40,   194,   195,  -181,  -181
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,     4,     1,     0,     3,     0,    43,
       5,    41,     3,     3,     0,    55,     6,     0,    53,     0,
       8,     0,    54,     3,    52,     3,     0,    56,    72,    66,
      67,    68,    47,     0,    71,     0,     0,    61,     3,     0,
       0,     0,     9,    12,    13,     0,     3,    48,     7,     0,
       0,    60,     3,    51,     0,    10,    70,    69,    14,    21,
       0,     0,    15,    59,    58,    57,     0,    44,    62,    50,
       0,     0,     3,    17,    16,     0,     0,     3,     3,    11,
       0,    24,     0,     0,     0,     0,     3,    65,    64,    63,
      42,    23,    22,    18,    20,     3,    80,    83,     0,    99,
       0,     0,     0,     0,     0,    73,    95,     0,     0,     3,
       3,     3,     3,     3,     3,     3,   116,   117,     3,     3,
       3,     0,   102,   110,   158,   161,     3,    98,    96,    49,
      89,    88,    90,    91,    92,    93,    94,    25,    87,    86,
       0,     0,    31,   150,   151,   152,   148,     0,   120,   132,
     139,     0,     0,     3,     3,     3,     0,    75,    33,    79,
       3,    78,    74,     3,     3,     0,     3,     3,   100,   121,
     123,   124,   125,   126,   122,     3,     3,     3,   157,   155,
     156,   154,   149,   153,     0,   104,     0,   107,     0,     0,
     162,     0,     0,     3,    77,     0,    26,    81,     0,     0,
     118,   134,   136,   138,   133,   141,   143,   145,   147,   140,
     101,     0,     0,   108,   111,   159,     3,    76,    36,    34,
      97,     3,     3,     0,    32,     3,     3,     3,     3,     3,
       3,     3,   103,     3,     0,     0,   163,     3,    28,    30,
       0,     3,    85,    82,    45,   127,   129,   131,   119,   135,
     137,   142,   144,   146,   106,   109,     0,     0,     0,    37,
      40,     0,     3,    27,    84,     3,     3,     3,     3,     3,
     164,     3,    35,    29,    46,   128,   130,     0,     0,     3,
      38,     3,   160,   165,     3,   113,   115,   112,     0,    39,
       0,     0,     3,     3,     0,     0,   114,   166
  };

  const short
  parser::yypgoto_[] =
  {
    -181,  -181,    -7,  -181,  -181,  -181,  -181,   -34,  -181,   115,
    -181,  -181,   105,   122,  -181,  -181,  -181,  -181,  -181,    83,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,   -80,
    -181,  -181,   133,  -181,   -49,  -181,  -181,  -181,  -181,  -181,
    -181,    87,   166,  -181,  -181,  -181,   149,  -181,  -181,   -39,
      42,  -181,   -77,  -181,    48,  -181,   -31,  -181,  -181,  -181,
    -181,  -102,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -180,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -106,
    -181,  -181,  -181,  -181,  -181,  -154,  -181,  -181,  -181,  -181,
    -101,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,   145,     3,     6,    12,    23,    21,    26,    42,
      71,    43,    44,    62,    83,    84,    45,    72,    82,   160,
     164,   221,   240,   262,   146,   167,   161,   193,   237,   261,
     271,   284,    10,    13,    33,    76,   265,    48,    66,    54,
      19,    16,    22,    46,    65,    38,    51,    77,    89,    34,
      58,    35,   107,   126,   162,   191,    98,   118,   222,   243,
     140,   108,   109,   163,   110,   111,   120,   112,   113,   153,
     186,   211,   233,   114,   154,   234,   287,   290,   115,   187,
     225,   175,   248,   266,   267,   148,   176,   204,   226,   227,
     149,   177,   209,   228,   229,   230,   150,   151,   182,   183,
     116,   155,   235,   117,   156,   216,   258,   279,   288
  };

  const short
  parser::yytable_[] =
  {
       9,    57,    50,    69,     1,    15,    18,   130,   131,   132,
     133,   134,   135,   136,   147,   142,    32,     4,    37,   127,
       5,   200,    74,     7,   143,   144,   142,   125,    29,    30,
      31,    53,    11,   128,   157,   143,   144,     8,    56,    63,
     137,    14,   158,    50,   152,    32,   201,   202,   188,   189,
      39,   254,    40,   255,   185,    41,    17,   195,   196,   245,
     246,   199,   236,    25,   198,    81,    28,    29,    30,    31,
      87,     9,   249,   250,   181,    99,   100,   101,    60,    97,
      61,    20,   102,    27,   103,   104,    24,   219,   106,   105,
     169,   170,   171,   172,   173,   174,   105,    36,   178,   179,
     180,    47,   106,   106,   106,   106,   106,   106,   106,    49,
      52,   138,    15,    59,   217,    29,    30,    31,    55,   159,
     205,   206,   207,    68,    70,   283,    57,   251,   252,   253,
      67,    73,    75,    78,    80,   -19,    85,    91,    86,    92,
      41,    95,    96,   105,   119,   165,   121,   122,   123,   124,
     129,   192,   137,   159,   166,   168,   273,   184,    -3,   106,
     275,   276,   210,   190,  -105,   280,   277,   278,   212,   203,
     208,   213,   223,   214,   215,   218,   220,   224,   231,   263,
     232,   238,   241,   268,   244,   259,    79,   269,   256,    94,
     294,   295,   281,   282,   272,   257,   285,   270,   292,   291,
     293,   139,   296,   297,   289,    93,   141,   197,   194,     0,
     264,    90,    64,     0,   239,   242,   274,     0,   247,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
     260,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,     0,
       0,   106,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,   106,   106
  };

  const short
  parser::yycheck_[] =
  {
       7,    40,    36,    52,    27,    12,    13,   109,   110,   111,
     112,   113,   114,   115,   120,    11,    23,    49,    25,     5,
       0,   175,    61,     6,    20,    21,    11,   104,    39,    40,
      41,    38,    49,    19,     3,    20,    21,    28,    49,    46,
       9,    29,    11,    77,   121,    52,    20,    21,   154,   155,
       4,   231,     6,   233,    50,     9,    32,   163,   164,    25,
      26,   167,   216,     7,   166,    72,    38,    39,    40,    41,
      77,    78,   226,   227,   151,    35,    36,    37,     4,    86,
       6,    49,    42,     5,    44,    45,    49,   193,    95,    49,
      13,    14,    15,    16,    17,    18,    49,    33,    51,    52,
      53,    30,   109,   110,   111,   112,   113,   114,   115,    34,
      31,   118,   119,    52,   191,    39,    40,    41,    49,   126,
      22,    23,    24,     5,     8,   279,   165,   228,   229,   230,
      49,    49,    11,     5,     4,     9,    12,    52,    11,    10,
       9,     7,    49,    49,    12,     6,    11,    11,    11,    11,
       8,   158,     9,   160,     7,     5,   262,    12,    12,   166,
     266,   267,     5,    19,     4,   271,   268,   269,    12,   176,
     177,     4,     8,    12,    12,    12,     5,    12,     4,    10,
       5,     4,     4,     7,     5,     4,    71,     7,    48,    84,
     292,   293,     8,     8,    12,    47,    43,    46,     7,    47,
       7,   118,     8,     8,   284,    83,   119,   165,   160,    -1,
     241,    78,    46,    -1,   221,   222,   265,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
     237,    -1,    -1,    -1,   241,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,
      -1,   268,   269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   281,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   292,   293
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    27,    55,    57,    49,     0,    58,     6,    28,    56,
      86,    49,    59,    87,    29,    56,    95,    32,    56,    94,
      49,    61,    96,    60,    49,     7,    62,     5,    38,    39,
      40,    41,    56,    88,   103,   105,    33,    56,    99,     4,
       6,     9,    63,    65,    66,    70,    97,    30,    91,    34,
      61,   100,    31,    56,    93,    49,    49,   103,   104,    52,
       4,     6,    67,    56,    96,    98,    92,    49,     5,    88,
       8,    64,    71,    49,   103,    11,    89,   101,     5,    63,
       4,    56,    72,    68,    69,    12,    11,    56,   100,   102,
      86,    52,    10,    67,    66,     7,    49,    56,   110,    35,
      36,    37,    42,    44,    45,    49,    56,   106,   115,   116,
     118,   119,   121,   122,   127,   132,   154,   157,   111,    12,
     120,    11,    11,    11,    11,   106,   107,     5,    19,     8,
     115,   115,   115,   115,   115,   115,   115,     9,    56,    73,
     114,    95,    11,    20,    21,    56,    78,   133,   139,   144,
     150,   151,   106,   123,   128,   155,   158,     3,    11,    56,
      73,    80,   108,   117,    74,     6,     7,    79,     5,    13,
      14,    15,    16,    17,    18,   135,   140,   145,    51,    52,
      53,   106,   152,   153,    12,    50,   124,   133,   133,   133,
      19,   109,    56,    81,   108,   133,   133,   104,   115,   133,
     139,    20,    21,    56,   141,    22,    23,    24,    56,   146,
       5,   125,    12,     4,    12,    12,   159,   106,    12,   133,
       5,    75,   112,     8,    12,   134,   142,   143,   147,   148,
     149,     4,     5,   126,   129,   156,   139,    82,     4,    56,
      76,     4,    56,   113,     5,    25,    26,    56,   136,   139,
     139,   144,   144,   144,   124,   124,    48,    47,   160,     4,
      56,    83,    77,    10,   110,    90,   137,   138,     7,     7,
      46,    84,    12,   133,    88,   133,   133,   115,   115,   161,
     133,     8,     8,   139,    85,    43,    56,   130,   162,    83,
     131,    47,     7,     7,   115,   115,     8,     8
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    54,    55,    56,    58,    59,    60,    57,    62,    61,
      64,    63,    63,    65,    65,    66,    67,    68,    67,    69,
      67,    71,    70,    72,    72,    74,    75,    73,    77,    76,
      76,    79,    78,    81,    82,    80,    80,    84,    85,    83,
      83,    87,    86,    86,    89,    90,    88,    88,    92,    91,
      93,    93,    94,    94,    95,    95,    97,    96,    98,    98,
      99,    99,   101,   100,   102,   102,   103,   103,   103,   104,
     104,   105,   105,   107,   106,   109,   108,   108,   108,   108,
     111,   112,   110,   110,   113,   113,   114,   114,   115,   115,
     115,   115,   115,   115,   115,   115,   117,   116,   118,   120,
     119,   121,   123,   122,   124,   125,   124,   124,   126,   124,
     128,   129,   127,   131,   130,   130,   132,   132,   134,   133,
     133,   135,   135,   135,   135,   135,   135,   137,   136,   138,
     136,   136,   140,   139,   142,   141,   143,   141,   141,   145,
     144,   147,   146,   148,   146,   149,   146,   146,   150,   150,
     151,   151,   151,   152,   152,   153,   153,   153,   155,   156,
     154,   158,   159,   160,   161,   162,   157
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     0,     0,     0,    10,     0,     3,
       0,     4,     1,     1,     2,     2,     2,     0,     4,     0,
       4,     0,     5,     2,     1,     0,     0,     6,     0,     3,
       1,     0,     4,     0,     0,     6,     3,     0,     0,     5,
       1,     0,    10,     1,     0,     0,    14,     1,     0,     7,
       2,     1,     2,     1,     2,     1,     0,     4,     1,     1,
       2,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     3,     2,     1,     1,
       0,     0,     7,     1,     2,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     1,     0,     5,     2,     0,
       4,     5,     0,     6,     1,     0,     4,     1,     0,     4,
       0,     0,    11,     0,     5,     1,     1,     1,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     0,     3,     1,     0,
       3,     0,     3,     0,     3,     0,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     0,
      10,     0,     0,     0,     0,     0,    15
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"eof\"", "error", "$undefined", "PUNTO", "COMA", "SCOL", "COL",
  "LCUR", "RCUR", "LBRA", "RBRA", "LPAR", "RPAR", "EQ", "GE", "LE", "GT",
  "LT", "NE", "ASSIGN", "SUM", "RES", "MUL", "DIV", "MOD", "AND", "OR",
  "PROGRAMA", "CLASE", "VARIABLES", "MAIN", "METODOS", "HEREDA",
  "ATRIBUTOS", "FUNCION", "RETORNAR", "LEER", "ESCRIBIR", "VOID", "INT_T",
  "FLOAT_T", "CHAR_T", "SI", "SINO", "MIENTRAS", "PARA", "EN", "HACER",
  "ENTONCES", "ID", "STRING", "CHAR", "INT", "FLOAT", "$accept", "script",
  "empty", "program", "$@1", "$@2", "$@3", "declare_var", "$@4",
  "declare_bridge1", "$@5", "declare_bridge2", "declare_bridge3",
  "declare_bridge4", "$@6", "$@7", "dimensionsdecl", "$@8", "matrixdecl",
  "dimensions", "$@9", "$@10", "matrix", "$@11", "single_express", "$@12",
  "mult_express", "$@13", "$@14", "expression_loop", "$@15", "$@16",
  "classes", "$@17", "functions", "$@18", "$@19", "main", "$@20",
  "methods", "inheritance", "variables", "var_bridge1", "$@21",
  "var_bridge2", "attributes", "attr_bridge1", "$@22", "attr_bridge2",
  "primitive_type", "var_type", "return_type", "call_path", "$@23",
  "call_options", "$@24", "parameters", "$@25", "$@26", "par_cont",
  "par_array", "statutes", "assignment", "$@27", "call_void",
  "function_return", "$@28", "read", "write", "$@29",
  "write_expression_opt", "$@30", "$@31", "decision_statement", "$@32",
  "$@33", "dec_else", "$@34", "repetition_statement", "expression", "$@35",
  "relop", "express_loop", "$@36", "$@37", "exp", "$@38", "exp_loop",
  "$@39", "$@40", "term", "$@41", "term_loop", "$@42", "$@43", "$@44",
  "factor", "sign", "call", "var_cte", "conditional", "$@45", "$@46",
  "nonconditional", "$@47", "$@48", "$@49", "$@50", "$@51", YY_NULLPTR
  };

#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   136,   136,   139,   142,   144,   148,   142,   155,   155,
     156,   156,   157,   158,   159,   160,   161,   162,   162,   163,
     163,   168,   168,   169,   170,   173,   202,   173,   276,   276,
     313,   318,   318,   337,   341,   337,   362,   364,   368,   364,
     389,   392,   392,   399,   402,   419,   402,   422,   425,   425,
     432,   433,   436,   437,   442,   443,   446,   445,   463,   464,
     471,   472,   473,   473,   482,   483,   488,   489,   490,   493,
     493,   496,   496,   499,   499,   572,   572,   573,   574,   597,
     602,   603,   602,   608,   610,   611,   613,   614,   618,   619,
     620,   621,   622,   623,   624,   625,   629,   628,   664,   667,
     667,   693,   702,   702,   703,   710,   710,   716,   735,   735,
     756,   756,   756,   768,   768,   777,   783,   783,   786,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   794,   795,
     795,   796,   799,   799,   816,   816,   817,   817,   818,   821,
     821,   838,   838,   839,   839,   840,   840,   841,   844,   845,
     846,   847,   848,   849,   850,   853,   854,   855,   858,   858,
     858,   885,   890,   897,   905,   905,   885
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 6 "parser.yy"
} // yy
#line 2374 "parser.tab.cc"

#line 932 "parser.yy"

void yy::parser::error(const location_type& lugar, const std::string& lexema)
{

  throw CompilerError("Line: "+ std::to_string(yylineno) + " :: " + lexema);
}
